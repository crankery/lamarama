$fn = 32;

union() {
  difference() {
    color("navy")
      cube([37.1, 62.0, 1.2]);
      
    translate([4.5, 2, 0])
      cylinder(1.2, r = 2.5/2);
      
     translate([37.1-4.5, 2, 0])
      cylinder(1.2, r = 2.5/2);
      
    translate([4.5, 60, 0])
      cylinder(1.2, r = 2.5/2);
      
    translate([37.1-4.5, 60, 0])
      cylinder(1.2, r = 2.5/2);
  }
  
  difference() {
    translate([(37.1-35.7)/2,5.4,-4+1.2])
      color("white")
        cube([35.7,51.2,4-1.2]);
          
      translate([(37.1-30.8)/2, 2.44+5.4, -4+1.2])
        cube([30.8,40.8,1]);
   }
  
   color("grey") 
     translate([(37.1-30.8)/2, 2.44+5.4, -3.9+1.2])
        cube([30.8,40.8,.9]);
        
    color("black", 0.25)
       translate([(37.1-35.7)/2,5.4, 1.21])
        cube([35.7, 51.2, 2]);
}