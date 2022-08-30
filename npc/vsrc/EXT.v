module EXT (
    input [31:0] inst,

    output [63:0] immJ,
    output [63:0] immB,
    output [63:0] immI,
    output [63:0] immS,
    output [63:0] immU,
    output [4:0] rs2,
    output [4:0] rs1,
    output [4:0] rd
);

assign immJ = {43{inst[31]},inst[31],inst[19:12],inst[20],inst[30:21],1'b0};
assign immB = {51{inst[31]},inst[31],inst[7],inst[30:25],inst[11:8],1'b0};
assign immI = {52{inst[31]},inst[31:20]};
assign immS = {52{inst[31]},inst[31:25],inst[11:7]};
assign immU = {32'b0,inst[31:12],12'b0};

assign rs2 = {inst[24:20]};
assign rs1 = {inst[19:15]};
assign rd  = {inst[11:7]};

endmodule