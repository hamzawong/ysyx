module IFU (
    input clk,
    input rst_n,
    input j,
    input [63:0] jPC,

    output [31:0] inst,
    output [63:0] snpc,
    output [63:0] PC,
);

reg [63:0] r_pc;

always@(posedge clk or negedge rst_n)begin
    if(~rst_n)begin
        r_pc <= 64'd4;
    end
    else if(j)begin
        r_pc <= jPC;
    end
    else begin
        r_pc <= r_pc + 4;
    end
end

assign PC = r_pc;
assign snpc = r_pc + 4;

ROM u_ROM_INST(
    .ain_ROM(PC[19:0]),
    .dout_ROM(inst)
)

endmodule