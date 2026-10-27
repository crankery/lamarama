$fn = 32;

difference() {
  union() {
    cylinder(h = 4.5, r = 5.5/2);
    cylinder(h = 0.5, r = 7.04/2);
  }
  
  cylinder(h = 3.5, r = 3.1 / 2);
}