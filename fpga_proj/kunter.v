module up_counter(
	output reg[7:0] out,
	input enable,
	input clk,
	input reset
);
	always @(posedge clk)
	begin
		if (reset)
			out <= 8'b0;
		else if (enable)
			out <= out + 1;
	end
endmodule
