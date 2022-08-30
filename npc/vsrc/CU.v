module CU (
    //IFU
    input           jump,
    input           jumpr,
    input           branch,
    //input   [63:0]  dout_ALU,
    input   [63:0]  immB,
    input   [63:0]  immJ,
    output          j,//c1
    output  [63:0]  jPC,
    //REGS
    input           RegWr,
    input   [4:0]   rs2,
    input   [4:0]   rs1,
    input   [4:0]   rd,
    input           immU_others,
    input           snpc_data,
    input           mem_result,
    input   [63:0]  immU,
    input   [63:0]  snpc,
    input   [63:0]  dout_MEM,
    input   [63:0]  dout_ALU,
    output          wen_REGS,//c2
    output  [63:0]  din_REGS,
    output  [4:0]   ain1_REGS,
    output  [4:0]   ain2_REGS,
    output  [4:0]   aind_REGS,
    //ALU
    input   [4:0]   ALUCtrl,
    input           hloutalu,
    input           lenoutalu,
    input           immU_rs1,
    input           PC_others,
    input           rs2_immSI,
    input           immS_immI,
    //input   [63:0]  immU,
    input   [63:0]  dout1_REGS,
    input   [63:0]  PC,
    input   [63:0]  dout2_REGS,
    input   [63:0]  immS,
    input   [63:0]  immI,
    output  [4:0]   ctrl_ALU,//c3
    output          hloutalu2,
    output          lenoutalu2,
    output  [63:0]  din1_ALU,
    output  [63:0]  din2_ALU,

    //MEM
    input           MemWr,
    input   [3:0]   lenoutmem,//TODO
    input           suoutmem,//TODO
    //input   [63:0]  dout_ALU,
    //input   [63:0]  dout2_REGS,
    output          wen_MEM,//c4
    output  [3:0]   lenoutmem2,
    output          suoutmem2,
    output  [63:0]  ain_MEM,
    output  [63:0]  din_MEM
);

//IFU
assign j    = jump  |   jumpr   |   (branch && dout_ALU == 1);
assign jPC  = (jumpr)?dout_ALU:(branch)?immB:immJ;

//REGS
assign wen_REGS = RegWr;
assign ain1_REGS = rs1;
assign ain2_REGS = rs2;
assign aind_REGS = rd;
assign din_REGS = (immU_others)?immU:(snpc_data)?snpc:(mem_result)?dout_MEM:dout_ALU;
//ALU
assign ctrl_ALU = ALUCtrl;
assign hloutalu2    = hloutalu;
assign lenoutalu2   = lenoutalu;
assign din1_ALU = (immU_rs1)?immU:dout1_REGS;
assign din2_ALU = (PC_others)?PC:(rs2_immSI)?dout2_REGS:(immS_immI)?immS:immI;
//Memory
assign wen_MEM = MemWr;
assign ain_MEM = dout_ALU;
assign din_MEM = dout2_REGS;

endmodule