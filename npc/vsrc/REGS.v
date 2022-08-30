module REGS (
    input   clk,
    input   rst_n,
    input           wen_REGS,
    input   [4:0]   ain1_REGS,
    input   [4:0]   ain2_REGS,
    input   [4:0]   aind_REGS,
    input   [63:0]  din_REGS,
    
    output  [63:0]  dout1_REGS,
    output  [63:0]  dout2_REGS

    

);

reg [63:0]  regs[4:0];

always@(posedge clk)begin
    if(wen_REGS)begin
        regs[aind_REGS] <= din_REGS;
    end
end

assign dout1_REGS = regs[ain1_REGS];
assign dout2_REGS = regs[ain2_REGS];
    
endmodule