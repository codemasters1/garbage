module Ws2812Driver(
  input clk_400k,
  input start,
  input has_next,
  input [7:0] r,
  input [7:0] g,
  input [7:0] b,
  output dout,
  output busy
);
  reg q = 0;
  reg [7:0] state = 8'd255;
  reg [23:0] data;
  assign dout = q;
  assign busy = state != 8'd255;
    
  always @(posedge clk_400k)
    begin
      if (start && (state == 8'd255 || state > 8'd71 && has_next))
        begin
          data = {r, g, b};
          state = 0;
        end
	
      if (state > 8'd71)
        begin
          q <= 0;
        end
      else if (state % 3 == 0)
        begin
          q <= 1;
        end
      else if (state % 3 == 1)
        begin
          q <= data[23];
        end
      else if (state % 3 == 2)
        begin
          data <= (data << 1);
          q <= 0;
        end
      
      if (state != 8'd255)
      	state <= state + 1;
    end
endmodule
  