module IDU (
    input   [31:0]  inst;
    //IFU
    output          jump,
    output          jumpr,
    output          branch,
    output  [63:0]  immJ,
    output  [63:0]  immB,
    //REGS
    output          RegWr,
    output  [4:0]   rs2,
    output  [4:0]   rs1,
    output  [4:0]   rd,
    output          immU_others,
    output          snpc_data,
    output          mem_result,
    output  [63:0]  immU,
    //ALU
    output  [4:0]   ALUCtrl,
    output          hloutalu,
    output          lenoutalu,
    output          immU_rs1,
    output          PC_others,
    output          rs2_immSI,
    output          immS_immI,
    output  [63:0]  immS,
    output  [63:0]  immI,
    //Memory
    output          MemWr,
    output  [3:0]   lenoutmem,
    output          suoutmem
);
//解码单元
wire [6:0] funct7;
wire [5:0] funct6;
wire [2:0] funct3;
wire [6:0] opcod;

assign funct7 = inst[31:25];
assign funct6 = inst[31:26];
assign funct3 = inst[14:12];
assign opcode = inst[6:0];
//编码类型
wire R_Type;
wire I_Type;
wire S_Type;
wire B_Type;
wire U_Type;
wire J_Type;
wire N_Type;

always@(*)begin
    case(opcode)
        7'b0110011:R_Type = 1;
        7'b0111011:R_Type = 1;
        7'b1100111:I_Type = 1;
        7'b0000011:I_Type = 1;
        7'b0010011:I_Type = 1;
        7'b0011011:I_Type = 1;
        7'b0100011:S_Type = 1;
        7'b1100011:B_Type = 1;
        7'b0110111:U_Type = 1;
        7'b0010111:U_Type = 1;
        7'b1101111:J_Type = 1;
    endcase
end
//funct3
wire funct3_Type0;
wire funct3_Type1;
wire funct3_Type2;
wire funct3_Type3;
wire funct3_Type4;
wire funct3_Type5;
wire funct3_Type6;
wire funct3_Type7;

always@(*)begin
    case(funct3)
        3'b000:funct3_Type0 = 1;
        3'b001:funct3_Type1 = 1;
        3'b010:funct3_Type2 = 1;
        3'b011:funct3_Type3 = 1;
        3'b100:funct3_Type4 = 1;
        3'b101:funct3_Type5 = 1;
        3'b110:funct3_Type6 = 1;
        3'b111:funct3_Type7 = 1;
    endcase
end
//funct6
wire funct6_Type0;
wire funct6_Type16;

always@(*)begin
    case(funct6)
        6'b000000:funct6_Type0 = 1;
        6'b010000:funct6_Type16= 1;
    endcase
end

//funct7
wire funct7_Type0;
wire funct7_Type1;
wire funct7_Type4;
wire funct7_Type8;
wire funct7_Type12;
wire funct7_Type16;
wire funct7_Type20;
wire funct7_Type32;
wire funct7_Type80;
wire funct7_Type96;
wire funct7_Type104;
wire funct7_Type112;
wire funct7_Type120;

always@(*)begin
    case(funct7)
        7'b0000000:funct7_Type0=1;
        7'b0000001:funct7_Type1=1;
        7'b0000100:funct7_Type4=1;
        7'b0001000:funct7_Type8=1;
        7'b0001100:funct7_Type12=1;
        7'b0010000:funct7_Type16=1;
        7'b0010100:funct7_Type20=1;
        7'b0100000:funct7_Type32=1;
        7'b1010000:funct7_Type80=1;
        7'b1100000:funct7_Type96=1;
        7'b1101000:funct7_Type104=1;
        7'b1110000:funct7_Type112=1;
        7'b1111000:funct7_Type120=1;
    endcase
end
//RV63IM指令集
wire LUI ,AUIPC,JAL   ,JALR  ,BEQ   ,BNE   ,BLT   ,BGE   ,BLTU  ,BGEU  ,
    LB    ,LH    ,LW    ,LBU   ,LHU   ,SB    ,SH    ,SW    ,
    ADDI  ,SLTI  ,SLTIU ,
    XORI  ,ORI   ,ANDI  ,
    SLLI  ,SRLI  ,SRAI  ,
    ADD   ,SUB   ,SLL   ,SLT   ,SLTU  ,
    XOR   ,SRL   ,SRA   ,OR    ,AND   ,
    LWU   ,LD    ,
    ADDIW ,SLLIW ,SRLIW ,SRAIW ,
    MUL   ,MULH  ,MULHSU,MULHU ,DIV   ,DIVU  ,REM   ,REMU  ,
    MULW  ,DIVW  ,DIVUW ,REMW  ,REMUW ;

assign LUI      = U_Type;
assign AUIPC    = U_Type;
assign JAL      = J_Type;
assign JALR     = I_Type;
assign BEQ      = B_Type    & funct3_Type0;
assign BNE      = B_Type    & funct3_Type1;
assign BLT      = B_Type    & funct3_Type4;
assign BGE      = B_Type    & funct3_Type5;
assign BLTU     = B_Type    & funct3_Type6;
assign BGEU     = B_Type    & funct3_Type7;
assign LB       = I_Type    & funct3_Type0;
assign LH       = I_Type    & funct3_Type1;
assign LW       = I_Type    & funct3_Type2;
assign LBU      = I_Type    & funct3_Type4;
assign LHU      = I_Type    & funct3_Type5;
assign SB       = S_Type    & funct3_Type0;
assign SH       = S_Type    & funct3_Type1;
assign SW       = S_Type    & funct3_Type2;
assign ADDI     = I_Type    & funct3_Type0;
assign SLTI     = I_Type    & funct3_Type2;
assign SLTIU    = I_Type    & funct3_Type3;
assign XORI     = I_Type    & funct3_Type4;
assign ORI      = I_Type    & funct3_Type6;
assign ANDI     = I_Type    & funct3_Type7;
assign SLLI     = I_Type    & funct3_Type1 & funct6_Type0;
assign SRLI     = I_Type    & funct3_Type5 & funct6_Type0;
assign SRAI     = I_Type    & funct3_Type5 & funct6_Type16;
assign ADD      = R_Type    & funct3_Type0 & funct7_Type0;
assign SUB      = R_Type    & funct3_Type0 & funct7_Type32;
assign SLL      = R_Type    & funct3_Type1 & funct7_Type0;
assign SLT      = R_Type    & funct3_Type2 & funct7_Type0;
assign SLTU     = R_Type    & funct3_Type3 & funct7_Type0;
assign XOR      = R_Type    & funct3_Type4 & funct7_Type0;
assign SRL      = R_Type    & funct3_Type5 & funct7_Type0;
assign SRA      = R_Type    & funct3_Type5 & funct7_Type32;
assign OR       = R_Type    & funct3_Type6 & funct7_Type0;
assign AND      = R_Type    & funct3_Type7 & funct7_Type0;
assign LWU      = I_Type    & funct3_Type6;
assign LD       = I_Type    & funct3_Type3;
assign ADDIW    = I_Type    & funct3_Type0;
assign SLLIW    = I_Type    & funct3_Type1 & funct7_Type0;
assign SRLIW    = I_Type    & funct3_Type5 & funct7_Type0;
assign SRAIW    = I_Type    & funct3_Type5 & funct7_Type32;
assign MUL      = R_Type    & funct3_Type0 & funct7_Type1;
assign MULH     = R_Type    & funct3_Type1 & funct7_Type1;
assign MULHSU   = R_Type    & funct3_Type2 & funct7_Type1;
assign MULHU    = R_Type    & funct3_Type3 & funct7_Type1;
assign DIV      = R_Type    & funct3_Type4 & funct7_Type1;
assign DIVU     = R_Type    & funct3_Type5 & funct7_Type1;
assign REM      = R_Type    & funct3_Type6 & funct7_Type1;
assign REMU     = R_Type    & funct3_Type7 & funct7_Type1;
assign MULW     = R_Type    & funct3_Type0 & funct7_Type1;
assign DIVW     = R_Type    & funct3_Type4 & funct7_Type1;
assign DIVUW    = R_Type    & funct3_Type5 & funct7_Type1;
assign REMW     = R_Type    & funct3_Type6 & funct7_Type1;
assign REMUW    = R_Type    & funct3_Type7 & funct7_Type1;
//根据指令生成控制信号
wire jump,jumpr,branch,RegWr,immU_others,snpc_data,mem_result,ALUCtrl,hloutalu,lenoutalu,immU_rs1,PC_others,rs2_immSI,immS_immI,MemWr,lenoutmem,suoutmem;

assign jump         = JAL;
assign jumpr        = JALR;
assign branch       = BEQ | BNE | BLT | BGE | BLTU | BGEU;
assign Regwr        = LUI AUIPC | JAL  | JALR | LB | LH | LW | LBU | LHU | ADDI | SLTI | SLTIU | XORI  | ORI  | ANDI  | 
                        SLLI | SRLI |  SRAI | ADD | SUB | SLL | SLT  | SLTU | XOR | SRL | SRA | OR | AND | LWU | LD | 
                        ADDIW | SLLIW | SRLIW | SRAIW | ADDW | SUBW | SLLW  | SRLW | SRAW | MUL | MULH | MULHSU | MULHU | 
                        DIV |  DIVU | REM | REMU | MULW  | DIVW | DIVUW | REMW | REMUW;
assign immU_others  = LUI;
assign snpc_data    = JAL | JALR;
assign mem_result   = LB | LH | LW | LBU | LHU  | LWU  | LD;
assign ALUCtrl      = (AUIPC | JALR | LB | LH | LW | LBU | LHU | SB | SH | SW | ADDI | ADD | LWU | LD | SD)?0:
                        (SUB)?1:
                        (XORI | XOR)?2:
                        (ORI | OR)?3:
                        (ANDI | AND)?4:
                        (SLLI | SLL)?5:
                        (SRLI | SRL)?6:
                        (SRAI | SRA)?7:
                        (BEQ)?8:
                        (BNE)?9:
                        (BLTU | SLTIU)?10:
                        (BGEU)?11:
                        (BLT | SLTI | SLT)?12:
                        (BGE)?13:
                        (SLLIW | SLLW)?14:
                        (SRLIW | SRLW)?15:
                        (SRAIW | SRAW)?16:
                        (ADDIW | ADDW)?17:
                        (SUBW)?18:
                        (MUL | MULH)?19:
                        (MULHSU)?20:
                        (MULHU)?21:
                        (MULW)?22:
                        (DIV)?23:
                        (DIVU)?24:
                        (DIVW)?25:
                        (DIVUW)?26:
                        (REM)?27:
                        (REMU)?28:
                        (REMW)?29:
                        (REMUW)?30:0;
assign hloutalu     = MULH | MULHSU | MULHU;
assign lenoutalu    = (ADDIW | SLLIW | SRLIW | SRAIW | ADDW | SUBW | SLLW | SRLW | SRAW | MULW | DIVW | DIVUW | REMW | REMUW)?4:8;
assign immU_rs1     = AUIPC;
assign PC_others    = AUIPC;
assign rs2_immSI    = BEQ | BNE | BLT | BGE | BLTU | BGEU | ADD | SUB | SLL | SLT  | SLTU | XOR | SRL | SRA | OR | AND | 
                        ADDW | SUBW | SLLW  | SRLW | SRAW | MUL | MULH | MULHSU | MULHU | 
                        DIV |  DIVU | REM | REMU | MULW  | DIVW | DIVUW | REMW | REMUW;
assign immS_immI    = SB | SH | SW | SD;
assign MemWr        = SB | SH | SW | SD;
assign lenoutmem    = (LB | LBU | SB)?1:
                        (LH | LHU | SH)?2:
                        (LW | SW | LWU)?4:
                        (LD | SD)?8:0;
assign suoutmem     = (LB | LH | LW | LD)?1:0;

//位扩展
EXT u_EXT(
    .inst(inst)

    .immJ(immJ),
    .immB(immB),
    .immI(immI),
    .immS(immS),
    .immU(immU),
    .rs2(rs2),
    .rs1(rs1),
    .rd(rd),
);

endmodule
