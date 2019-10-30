import math
import numpy as np


class Projector:
    def __init__(self, dia=500, fov=20, x0=0, y0=0, pan=0, tilt=0):
        self.f = 0.5 * dia / math.tan(math.radians(fov)/2)
        self.K = np.matrix([[self.f, 0, x0],
                            [0, self.f, y0],
                            [0,      0,  1]], dtype=np.float64)  # intrinsic parameters

        self.Rt = self.get_rotation_matrix(pan, tilt)
        self.H = np.matmul(np.append(self.K, [[0], [0], [0]], axis=1), self.Rt)

    @staticmethod
    def get_rotation_matrix(pan, tilt, roll=0):
        pan_r = math.radians(pan)
        tilt_r = math.radians(tilt)
        roll_r = math.radians(roll)

        Rx = np.matrix([[1, 0, 0],
                        [0, math.cos(roll_r), -math.sin(roll_r)],
                        [0, math.sin(roll_r), math.cos(roll_r)]], dtype=np.float64)
        Ry = np.matrix([[math.cos(tilt_r), 0, math.sin(tilt_r)],
                        [0, 1, 0],
                        [-math.sin(tilt_r), 0, math.cos(tilt_r)]], dtype=np.float64)
        Rz = np.matrix([[math.cos(pan_r), -math.sin(pan_r), 0],
                        [math.sin(pan_r), math.cos(pan_r), 0],
                        [0, 0, 1]], dtype=np.float64)

        R = np.matmul(np.matmul(Rz, Ry), Rx)
        Rt = np.append(np.matmul(R, np.eye(3, 4)), [[0, 0, 0, 1]], axis=0)
        return np.matrix(Rt)

    def get_camera_coords(self, x, y, z):
        world_coords = np.transpose(np.asmatrix([x, y, z, 1], dtype=np.float64))
        camera_coords = np.matmul(self.H, world_coords)
        return float(camera_coords[0]/camera_coords[2]), float(camera_coords[1]/camera_coords[2])
