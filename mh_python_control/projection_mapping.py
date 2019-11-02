import math
import numpy as np
import cv2


class Projector:
    def __init__(self, dia=500, fov=20, x0=0, y0=0, pan=0, tilt=0, roll=0):
        self.f = 0.5 * dia / math.tan(math.radians(fov)/2)
        self.K = np.matrix([[self.f, 0, x0],
                            [0, self.f, y0],
                            [0,      0,  1]], dtype=np.float64)  # intrinsic parameters

        self.Rt = self.get_rotation_matrix(pan, tilt, roll)
        self.H = np.matmul(np.append(self.K, [[0], [0], [0]], axis=1), self.Rt)
        self.x0 = x0
        self.y0 = y0
        self.pan = pan
        self.tilt = tilt
        self.roll = roll

    @staticmethod
    def get_rotation_matrix(pan, tilt, roll=0):
        pan_r = math.radians(pan)
        tilt_r = math.radians(tilt)
        roll_r = math.radians(roll)

        Rx = np.matrix([[1, 0, 0],
                        [0, math.cos(tilt_r), -math.sin(tilt_r)],
                        [0, math.sin(tilt_r), math.cos(tilt_r)]], dtype=np.float64)
        Ry = np.matrix([[math.cos(tilt_r), 0, math.sin(tilt_r)],
                        [0, 1, 0],
                        [-math.sin(tilt_r), 0, math.cos(tilt_r)]], dtype=np.float64)
        Rz = np.matrix([[math.cos(pan_r), -math.sin(pan_r), 0],
                        [math.sin(pan_r), math.cos(pan_r), 0],
                        [0, 0, 1]], dtype=np.float64)

        R = np.eye(3, 3, dtype=np.float64)
        R = np.matmul(Rz, R)
        R = np.matmul(Rx, R)

        Rt = np.append(np.matmul(R, np.eye(3, 4)), [[0, 0, 0, 1]], axis=0)
        return np.matrix(Rt)

    def set_rotation(self, pan=None, tilt=None, roll=None):
        if pan is None:
            pan = self.pan

        if tilt is None:
            tilt = self.tilt

        if roll is None:
            roll = self.roll
        
        self.pan = pan
        self.tilt = tilt
        self.roll = roll
        self.Rt = self.get_rotation_matrix(pan, tilt, roll)
        self.H = np.matmul(np.append(self.K, [[0], [0], [0]], axis=1), self.Rt)

    def get_camera_coords(self, x, y, z):
        world_coords = np.transpose(np.asmatrix([x, y, z, 1], dtype=np.float64))
        camera_coords = np.matmul(self.H, world_coords)
        x = float(camera_coords[0]/camera_coords[2])
        y = float(camera_coords[1]/camera_coords[2])

        # flip image
        y -= self.y0
        y = self.y0 - y
        return x, y

    def project_world_coords(self, cx, cy, x0, y0, z0, xn, yn, zn):
        # TODO
        pass


class Texture:
    def __init__(self, img_path, x, y, z, scale=1.0):
        self.img = cv2.imread(img_path)
        self.x = x
        self.y = y
        self.z = z
        self.h, self.w, _ = np.shape(self.img)
        self.scale = scale  # mm/px

    def get_camera_coords(self, projector):
        # coordinates of upper left corner
        x = self.x - self.w * 0.5 * self.scale
        y = self.y
        z = self.z + self.h * 0.5 * self.scale
        cx0, cy0 = projector.get_camera_coords(x, y, z)

        # upper right corner
        x += self.w * self.scale
        cx1, cy1 = projector.get_camera_coords(x, y, z)

        # lower left corner
        x -= self.w * self.scale
        z -= self.h * self.scale
        cx2, cy2 = projector.get_camera_coords(x, y, z)

        # lower right corner
        x += self.w * self.scale
        cx3, cy3 = projector.get_camera_coords(x, y, z)

        return cx0, cy0, cx1, cy1, cx2, cy2, cx3, cy3

    def render(self, projector, img):
        dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3 = self.get_camera_coords(projector)
        src = np.float32([[0, 0], [self.w, 0], [0, self.h], [self.w, self.h]])
        dst = np.float32([[dx0, dy0], [dx1, dy1], [dx2, dy2], [dx3, dy3]])
        mat = cv2.getPerspectiveTransform(src, dst)
        h, w, _ = np.shape(img)
        img = cv2.warpPerspective(self.img, mat, (w, h), img, borderMode=cv2.BORDER_TRANSPARENT)
        return img
