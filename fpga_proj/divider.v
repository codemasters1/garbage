//module divider(
//	input clk_in,
//	output clk_out
//);
//
//reg [31:0] counter;
//assign clk_out = counter[0];
//
//always @(posedge clk_in)
//begin
//	counter <= counter + 1;
//end
//
//endmodule

module divider(
	input clk_in, 
	output reg clk_out
);

parameter sys_clk = 10000000;
parameter out_clk = 2500000;
parameter max = sys_clk / (2 * out_clk);

reg [31:0] counter = 0;

always@(posedge clk_in)
begin
	if (counter == max - 1)
	begin
		counter <= 0;
		clk_out <= ~clk_out;
	end
	else
	begin
		counter <= counter + 1;
	end
end
endmodule
