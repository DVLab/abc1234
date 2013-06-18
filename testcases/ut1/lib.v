module adder4(in1, in2, sum);
  input  [3:0] in1;
  input  [3:0] in2;
  output [3:0] sum;
    assign sum = in1 + in2;
endmodule
