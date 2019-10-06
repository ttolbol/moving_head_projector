$fs=1.0;

center_plate();

module center_plate_2d(){
    difference(){
        hull(){
            square([103, 114.5], center=true);
            square([200, 50], center=true);
        }
        circle(d=30);
        
        // nema 17 motor mount
        translate([-13-(30+26)/2, 0]){
            circle(d=26);
            translate([31/2, 31/2]) circle(d=3.4);
            translate([-31/2, 31/2]) circle(d=3.4);
            translate([31/2, -31/2]) circle(d=3.4);
            translate([-31/2, -31/2]) circle(d=3.4);
        }
        
        // cable holes
        translate([22+(15+30)/2, 55/2]) circle(d=15);
        translate([22+(15+30)/2, -55/2]) circle(d=15);
        translate([22+(15+30)/2, 3+(55+15+3.4)/2]) circle(d=3.4);
        translate([22+(15+30)/2, -3-(55+15+3.4)/2]) circle(d=3.4);
        
        // mounting holes
        translate([0, (114.5-2.6)/2-5.5]) circle(d=3.0);
        translate([0, -(114.5-2.6)/2+5.5]) circle(d=3.0);
        
        translate([103/2, (114.5-2.6)/2-5.5]) circle(d=3.4);
        translate([-103/2, (114.5-2.6)/2-5.5]) circle(d=3.4);
    }
}

module center_plate(t=2.3){
    translate([0,0,-t]) linear_extrude(height=t){
        center_plate_2d();
    }
}