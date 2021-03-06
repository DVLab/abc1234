module top(a, sel, out);
  input  [2:0] a;
  input        sel;
  output [1:0] out;
  wire [2:0] a;
  wire       sel;
  wire [1:0] out;
  wire [3:0] z;
  wire [4:0] tmp;
  wire w4, w5, w6, w7;
    adder4 u1(4'b0011, {1'b0, a}, tmp);
    not (z[3], tmp[3]);
    not (z[2], tmp[2]);
    buf (z[1], tmp[1]);
    buf (z[0], tmp[0]);
    nor (w4, sel, z[3]);
    nor (w5, sel, z[2]);
    and (w6, sel, z[1]);
    and (w7, sel, z[0]);
    or (out[1], w4, w6);
    or (out[0], w5, w7);
endmodule
