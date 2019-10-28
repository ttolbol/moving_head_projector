use <mipi_controller.scad>

module pcb_bracket_large(){
    difference(){
        union(){
            hull(){
                translate([-3.5,0,-1.5]) cube([58,7.5,3], center=true);
                translate([-3.5,2,5.5]) cube([58,3,7], center=true);
            }
        }
        translate([32.5/2,0,0]) cylinder(d=3.5, h=30, center=true, $fn=16);
        translate([-32.5/2,0,0]) cylinder(d=3.5, h=30, center=true, $fn=16);
        
        translate([32.5/2,0,0]) cylinder(d=6, h=10, $fn=16);
        translate([-32.5/2,0,0]) cylinder(d=6, h=10, $fn=16);
        
        translate([-3.9,2,5.5]){
            translate([49.2/2,0,0]) rotate([90,0,0]) cylinder(d=3.5, h=10, center=true, $fn=16);
            translate([-49.2/2,0,0]) rotate([90,0,0]) cylinder(d=3.5, h=10, center=true, $fn=16);
            
            translate([49.2/2,-4,0]) rotate([90,0,0]) cylinder(d1=6, d2=7.5, h=5, center=true, $fn=6);
            translate([-49.2/2,-4,0]) rotate([90,0,0]) cylinder(d1=6, d2=7.5, h=5, center=true, $fn=6);
        }
        
    }
}

//translate([0,4,32.5+2]) rotate([-90,-90,0]) pcb_large();
pcb_bracket_large();