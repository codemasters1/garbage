module licznik_tb();

reg sys_clk;
wire cnt;

divider UUT(
.clk_in(sys_clk),
.clk_out(cnt)
);

initial
 begin
 sys_clk = 0;
 end

initial
 begin
 // wyświetlenie wyników symulacji
 $monitor ($time, " | %d", cnt);
 end

//-----------------------------------

always #25 sys_clk <= ~sys_clk;

//-----------------------------------
endmodule
