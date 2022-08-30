module top (
    input clk,
    input rst_n,
);
//IFU

wire    [31:0]  inst;
wire    [63:0]  snpc;
wire    [63:0]  PC;

IFU u_IFU(
    .clk(clk),
    .rst_n(rst_n),
    .j(j),
    .jPC(jPC)

    .inst(inst),
    .snpc(snpc),
    .PC(PC),
);

wire            jump;
wire            jumpr;
wire            branch;
wire    [63:0]  immJ;
wire    [63:0]  immB;

wire            RegWr;
wire    [4:0]   rs2;
wire    [4:0]   rs1;
wire    [4:0]   rd;
wire            immU_others;
wire            snpc_data;
wire            mem_result;
wire    [63:0]  immU;

wire    [3:0]   ALUCtrl;
wire            hloutalu;
wire            lenoutalu;
wire            immU_rs1;
wire            PC_others;
wire            rs2_immSI;
wire            immS_immI;
wire    [63:0]  immS;
wire    [63:0]  immI;

wire            MemWr;
wire    [3:0]   lenoutmem;
wire            suoutmem;

IDU u_IDU(
    .inst(inst)

    .jump(jump),
    .jumpr(jumpr),
    .branch(branch),
    .immJ(immJ),
    .immB(immB),

    .RegWr(RegWr),
    .rs1(rs1),
    .rs2(rs2),
    .rd(rd),
    .immU_others(immU_others) ,
    .snpc_data(snpc_data) ,
    .mem_result(mem_result),
    .immU(immU),

    .ALUCtrl(ALUCtrl),
    .hloutalu(hloutalu),
    .lenoutalu(lenoutalu),
    .immU_rs1(immU_rs1),
    .PC_others(PC_others),
    .rs2_immSI(rs2_immSI),
    .immS_immI(immS_immI),
    .immS(immS),
    .immI(immI),
    
    .MemWr(MemWr),
    .lenoutmem(lenoutmem),
    .suoutmem(suoutmem),
);

//CU
wire            j;
wire    [63:0]  jPC;

wire            wen_REGS;
wire    [63:0]  din_REGS;
wire    [4:0]   ain1_REGS;
wire    [4:0]   ain2_REGS;
wire    [4:0]   aind_REGS;

wire    [3:0]   ctrl_ALU;
wire    [63:0]  din1_ALU;
wire    [63:0]  din2_ALU;

wire            wen_MEM;
wire    [3:0]   lenoutmem2;
wire            suoutmem2;
wire    [19:0]  ain_MEM;
wire    [63:0]  din_MEM;

CU  u_CU(
    .jump(jump),
    .jumpr(jumpr),
    .branch(branch),
    .immB(immB),
    .immJ(immJ),
    .j(j),
    .jPC(jPC),
    
    .RegWr(RegWr),
    .rs2(rs2),
    .rs1(rs1),
    .rd(rd),
    .immU_others(immU_others),
    .snpc_data(snpc_data),
    .mem_result(mem_result),
    .immU(immU),
    .snpc(snpc),
    .dout_MEM(dout_MEM),
    .dout_ALU(dout_ALU),
    .wen_REGS(wen_REGS),
    .din_REGS(din_REGS),
    .ain1_REGS(ain1_REGS),
    .ain2_REGS(ain2_REGS),
    .aind_REGS(aind_REGS),

    .ALUCtrl(ALUCtrl),
    .hloutalu(hloutalu),
    .lenoutalu(lenoutalu),
    .immU_rs1(immU_rs1),
    .PC_others(PC_others),
    .rs2_immSI(rs2_immSI),
    .immS_immI(immS_immI),
    .immU(immU),
    .dout1_REG(dout1_REG),
    .PC(PC),
    .dout2_REG(dout2_REG),
    .immS(immS),
    .immI(immI),
    .ctrl_ALU(ctrl_ALU),
    .hloutalu2(hloutalu2),
    .lenoutalu2(lenoutalu2),
    .din1_ALU(din1_ALU),
    .din2_ALU(din2_ALU),

    .MemWr(MemWr),
    .lenoutmem(lenoutmem),
    .suoutmem(suoutmem)
    .wen_MEM(wen_MEM),
    .lenoutmem2(lenoutmem2),
    .suoutmem2(suoutmem2)
    .ain_MEM(ain_MEM),
    .din_MEM(din_MEM),
    
);

wire    [63:0]  dout1_REGS;
wire    [63:0]  dout2_REGS;

REGS u_REGS(
    .clk(clk),
    .rst_n(rst_n),
    .wen_REGS(wen_REGS),
    .ain1_REGS(ain1_REGS),
    .ain2_REGS(ain2_REGS),
    .aind_REGS(aind_REGS),
    .din_REGS(din_REGS),
    
    .dout1_REGS(dout1_REGS),
    .dout2_REGS(dout2_REGS),
);

wire    [63:0]  dout_MEM;

MEM u_MEM(
    .wen_MEM(wen_MEM),
    .ain_MEM(ain_MEM),
    .din_MEM(din_MEM),
    .lenoutmem(lenoutmem2),
    .suoutmem(suoutmem2),

    .dout_MEM(dout_MEM)
);

wire    [63:0]  dout_ALU;

ALU u_ALU(
    .ctrl_ALU(ALUCtrl),
    .hloutalu(hloutalu2),
    .lenoutalu(lenoutalu2),
    .din1_ALU(din1_ALU),
    .din2_ALU(din2_ALU),

    .dout_ALU(dout_ALU)
);
    
endmodule