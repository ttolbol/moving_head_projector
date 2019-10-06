use <mipi_controller.scad>;
use <center_plate.scad>;

foam_thickness = 16;

//translate([-18,0,34/2]) lcd();
//center_plate();

//translate([-35,0,0]) foam_bottom_left();
//translate([30,0,0]) foam_bottom_right();
//translate([26,0,3+34/2]) foam_strip();
//translate([-26,0,3+34/2]) foam_strip();

//lcd_holder_base_top();
//lcd_holder_base_bottom();

//translate([-26,0,21]) lcd_holder_cover_left();
translate([26,0,21]) lcd_holder_cover_right();

module foam_bottom_right(){
    color("Gray") linear_extrude(height=foam_thickness){
        translate([-29/2, -51/2]) hull(){
            square([17, 51]);
            translate([0,(51-26)/2]) square([29, 26]);
        }
    }
}

module foam_bottom_left(){
    color("Gray") translate([0,0, foam_thickness/2]) 
        cube([38, 60, foam_thickness], center=true);
}

module foam_strip(){
    color("Gray") cube([20,90,5.5], center=true);
}

module lcd_holder_base_bottom(){
    mirror([0,1,0]) lcd_holder_base_top();
}

module lcd_holder_base_top(){
    color("Green") translate([0,52,3]){
        difference(){
            union(){
                cube([112, 15, 6], center=true);
                translate([0,15/2-1,-2]) cube([112, 2, 10], center=true);
                
                translate([26,0,3]) lcd_holder_cover_mount();
                translate([-26,0,3]) lcd_holder_cover_mount();
                
                hull(){
                    translate([-112/2,0,0]) cube([0.5, 15, 6], center=true);
                    translate([-112/2-30,-20,0]) cube([0.5, 15, 6], center=true);
                }
                translate([-112/2-30,-20-7.5, 3]) cube([15,15,15]);
            }
            // mounting holes
            translate([0,-1.55,0]) cylinder(d=3.5, h=12, center=true, $fn=10);
            translate([103/2,-1.55,0]) cylinder(d=3.5, h=12, center=true, $fn=10);
            translate([-103/2,-1.55,0]) cylinder(d=3.5, h=12, center=true, $fn=10);
            
            translate([0,-1.55,-3]) cylinder(d=6.5, h=2, $fn=10);
            translate([0,-1.55,0]) cylinder(d=6, h=4, $fn=10);
            translate([103/2,-1.55,0]) cylinder(d=6, h=4, $fn=10);
            translate([-103/2,-1.55,0]) cylinder(d=6, h=4, $fn=10);
            
            // corner cutout
            
            translate([2-112/2-30,-2-20-7.5, 2.99]) cube([15,15,16]);
        }
    }
}

module lcd_holder_cover_mount(){
    difference(){
        union(){
            translate([-26/2,1.5,0]) cube([26, 6, 10]);
            translate([-26/2,1.5-8,0]) cube([8, 8, 10]);
            translate([26/2-8,1.5-8,0]) cube([8, 8, 10]);
            translate([-26/2,1.5-8,10]) cube([26, 14, 7]);
        }
        translate([0,-2,13]) cylinder(d=3.5, h=14, center=true, $fn=12);
        translate([0,-2,9]) cylinder(d2=6, d1=6.8, h=3, $fn=6);
        translate([-21/2,-8,17-2]) cube([21,12, 4]);
    }
}

module lcd_holder_cover_left(){
    color("Lime") difference(){
        union(){
            translate([0,0,3/2]) cube([20, 110, 3], center=true);
            hull(){
                translate([0,0,3/2]) cube([3, 90, 3], center=true);
                translate([0,0,3+3/2]) cube([3, 70, 3], center=true);
            }
        }
        translate([0,50,0]) cylinder(d=3.5, h=8, center=true, $fn=12);
        translate([0,-50,0]) cylinder(d=3.5, h=8, center=true, $fn=12);
    }
}

module lcd_holder_cover_right(){
    lcd_holder_cover_left();
}