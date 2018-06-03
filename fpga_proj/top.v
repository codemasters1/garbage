module top(
	input clk,
	output dout,
	output [3:0] out,
	output tx
);

wire o;
wire q;
assign out = {3'b111, {~o}};

divider t (
	.clk_in(clk),
	.clk_out(o)
);

divider d (
	.clk_in(clk),
	.clk_out(q)
);

reg [23:0] dd = 24'b11111111_00000000_00000000;
reg [7:0] byte;
wire busy;
reg hn = 1;

Ws2812Driver ws(
	.clk_400k(q),
	.start(1),
	.has_next(hn),
	.dout(dout),
	.busy(busy),
	.r(byte),
	.g(0),
	.b(~byte)
);
	

//
//
//
//reg start;
//wire busy;
//reg [7:0] char;
//
//async_transmitter tr(
//	.clk(clk),
//	.TxD_start(start),
//	.TxD_data(char),
//	.TxD(tx),
//	.TxD_busy(busy)
//);
//
//reg [7:0] addr;
//wire [7:0] data;
//
//mem lol(
//	.address(addr),
//	.q(data),
//	.wren(0),
//	.clock(clk)
//);
//
//always @(posedge clk)
//begin
//	if (!busy)
//	begin
//		char <= data;
//		addr <= addr + 1;
//		start <= 1;
//	end
//end

	wire c10h;
	divider #(
		.sys_clk(10000000),
		.out_clk(2)
	) dvd(
		.clk_in(clk),
		.clk_out(c10h)
	);

	always @(posedge c10h)
	begin
		if (!busy)
		begin
		
		byte <= byte + 1;
		end
	end

endmodule
