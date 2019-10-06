pcb_thickness = 1.0;

hole_dia_small = 2.05;
hole_dia_large = 2.8;

corner_radius = 4.0;

pcb_large();

translate([60,0,0]) pcb_small();

translate([0,70,34/2]){
    lcd();
    cylinder(d=30, h=2, center=true);
}

module pcb_small(){
    difference(){
        union(){
            color("SteelBlue") cube([42.7, 24.0, pcb_thickness], center=true);
            translate([0,24/2-4.7/2-1.3,pcb_thickness/2]) flex_connector();
            color("DimGray") translate([2.5/2-42.7/2+3.8,12.5/2-24/2+3.8,0.5]) cube([2.5, 12.5, 1.0], center=true);
        }
        translate([(41-hole_dia_small)/2, (22.6-hole_dia_small)/2, 0]) 
            cylinder(d=hole_dia_small, h=pcb_thickness*3, center=true, $fn=16);
        translate([-(41-hole_dia_small)/2, (22.6-hole_dia_small)/2, 0]) 
            cylinder(d=hole_dia_small, h=pcb_thickness*3, center=true, $fn=16);
        translate([(41-hole_dia_small)/2, -(22.6-hole_dia_small)/2, 0]) 
            cylinder(d=hole_dia_small, h=pcb_thickness*3, center=true, $fn=16);
        translate([-(41-hole_dia_small)/2, -(22.6-hole_dia_small)/2, 0]) 
            cylinder(d=hole_dia_small, h=pcb_thickness*3, center=true, $fn=16);
    }
}

module pcb_large(){
    difference(){
        union(){
            color("SteelBlue") hull(){
                translate([(65-corner_radius)/2, (64-corner_radius)/2, 0]) 
                    cylinder(d = corner_radius, h = pcb_thickness, $fn=16, center=true);
                translate([-(65-corner_radius)/2, (64-corner_radius)/2, 0]) 
                    cylinder(d = corner_radius, h = pcb_thickness, $fn=16, center=true);
                translate([(65-corner_radius)/2, -(64-corner_radius)/2, 0]) 
                    cylinder(d = corner_radius, h = pcb_thickness, $fn=16, center=true);
                translate([-(65-corner_radius)/2, -(64-corner_radius)/2, 0]) 
                    cylinder(d = corner_radius, h = pcb_thickness, $fn=16, center=true);
            }
            translate([0,(64-4.7)/2-1.3,pcb_thickness/2]) flex_connector();
            // HDMI:
            translate([0, -(64-11.6)/2-1.3,(6+pcb_thickness)/2]) color("Silver") 
                cube([15,11.6, 6], center=true);
            // USB:
            translate([7.7-(65-8)/2, -(64-6)/2-0.6,(2.5+pcb_thickness)/2]) color("Silver") 
                cube([8, 6, 2.5], center=true);
            
        }
        translate([(55.3+hole_dia_large)/2, 2.1-(64-hole_dia_large)/2, 0])
            cylinder(d=hole_dia_large, h=pcb_thickness*3, $fn=16, center=true);
        translate([-(55.3+hole_dia_large)/2, 2.1-(64-hole_dia_large)/2, 0])
            cylinder(d=hole_dia_large, h=pcb_thickness*3, $fn=16, center=true);
        
        translate([(55.3+hole_dia_large)/2, hole_dia_large+46.4+2.1-(64-hole_dia_large)/2, 0])
            cylinder(d=hole_dia_large, h=pcb_thickness*3, $fn=16, center=true);
        translate([-(55.3+hole_dia_large)/2, hole_dia_large+46.4+2.1-(64-hole_dia_large)/2, 0])
            cylinder(d=hole_dia_large, h=pcb_thickness*3, $fn=16, center=true);
    }
}

module flex_connector(){
    color("White") translate([0,0,2.1/2]) cube([35.1, 4.7, 2.1], center=true);
}

module lcd(){
    color("Silver") cube([127.6, 69.85, 0.6], center=true);
}