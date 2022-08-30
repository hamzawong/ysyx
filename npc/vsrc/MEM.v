module MEM (
    input           clk,
    input           wen_MEM,
    input   [3:0]   lenoutmem,//1byte,2byte,4byte,8byte
    input           suoutmem,
    input   [19:0]  ain_MEM,
    input   [63:0]  din_MEM,

    output  [63:0]  dout_MEM,
    
    
    
);

reg [7:0] mem[19:0];

always@(posedge clk)begin
    if(wen)begin
        case(lenoutmem)
            4'b0001:mem[ain_MEM] = din_MEM[7:0];
            4'b0010:{mem[ain_MEM+1],mem[ain_MEM]} = din_MEM[15:0];
            4'b0100:{mem[ain_MEM+3],mem[ain_MEM+2],mem[ain_MEM+1],mem[ain_MEM]} = din_MEM[31:0];
            4'b1000:{mem[ain_MEM+7],mem[ain_MEM+6],mem[ain_MEM+5],mem[ain_MEM+4],mem[ain_MEM+3],mem[ain_MEM+2],mem[ain_MEM+1],mem[ain_MEM]} = din_MEM;
            default:mem[ain_MEM] = 0;
        endcase
    end
end

always@(*)begin
    case({suoutmem,lenoutmem})
        5'b00001:dout_MEM = {56'b0,mem[ain_MEM]};
        5'b00010:dout_MEM = {48'b0,mem[ain_MEM+1],mem[ain_MEM]};
        5'b00100:dout_MEM = {32'b0,mem[ain_MEM+3],mem[ain_MEM+2],mem[ain_MEM+1],mem[ain_MEM]};
        5'b01000:dout_MEM = {mem[ain_MEM+7],mem[ain_MEM+6],mem[ain_MEM+5],mem[ain_MEM+4],mem[ain_MEM+3],mem[ain_MEM+2],mem[ain_MEM+1],mem[ain_MEM]};
        5'b10001:dout_MEM = {56{mem[ain_MEM][7]},mem[ain_MEM]};
        5'b10010:dout_MEM = {48{mem[ain_MEM+1][7]},mem[ain_MEM+1],mem[ain_MEM]};
        5'b10100:dout_MEM = {32{mem[ain_MEM+3][7]},mem[ain_MEM+3],mem[ain_MEM+2],mem[ain_MEM+1],mem[ain_MEM]};
        5'b11000:dout_MEM = {mem[ain_MEM+7],mem[ain_MEM+6],mem[ain_MEM+5],mem[ain_MEM+4],mem[ain_MEM+3],mem[ain_MEM+2],mem[ain_MEM+1],mem[ain_MEM]};
        default:dout_MEM = 0;
    endcase
end

endmodule