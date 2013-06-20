module lib1(in, out);
  input  [31:0] in;
  output        out;
  assign out = &in;
endmodule

module lib2(in, out);
  input  [31:0] in;
  output        out;
  assign out = |in;
endmodule

module lib3(in, out);
  input  [31:0] in;
  output        out;
  assign out = ^in;
endmodule

module lib4(in, out);
  input  [31:0] in;
  output        out;
  assign out = ~&in;
endmodule

module lib5(in, out);
  input  [31:0] in;
  output        out;
  assign out = ~|in;
endmodule

module lib6(in, out);
  input  [31:0] in;
  output        out;
  assign out = ~^in;
endmodule

module lib8(in1, in2, out);
  input [16:0] in1;
  input [16:0] in2;
  output [16:0] out;
  assign out = in1 + in2;
endmodule

module lib9(in1, in2, in3, out);
  input [17:0] in1;
  input [17:0] in2;
  input [17:0] in3;
  output [17:0] out;
  assign out = in1 + in2 + in3;
endmodule

module lib10(in1, in2, in3, in4, out);
  input [17:0] in1;
  input [17:0] in2;
  input [17:0] in3;
  input [17:0] in4;
  output [17:0] out;
  assign out = in1 + in2 + in3 + in4;
endmodule

module lib11(in1, in2, out);
  input [15:0] in1;
  input [15:0] in2;
  output [15:0] out;
  assign out = in1 * in2;
endmodule

module lib12(in1, in2, in3, out);
  input [16:0] in1;
  input [16:0] in2;
  input [16:0] in3;
  output [16:0] out;
  assign out = (in1 + in2) * in3;
endmodule

module lib13(in1, in2, in3, in4, out);
  input [16:0] in1;
  input [16:0] in2;
  input [16:0] in3;
  input [16:0] in4;
  output [16:0] out;
  assign out = in1 * in2 + in3 * in4;
endmodule

module lib14(in1, in2, in3, in4, in5, in6, out);
  input [17:0] in1;
  input [17:0] in2;
  input [17:0] in3;
  input [17:0] in4;
  input [17:0] in5;
  input [17:0] in6;
  output [17:0] out;
  assign out = in1 * in2 + in3 * in4 + in5 * in6;
endmodule
