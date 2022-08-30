module ALU (
    input   [3:0]   ctrl_ALU,
    input           hloutalu,
    input           lenoutalu,
    input   [63:0]  din1_ALU,
    input   [63:0]  din2_ALU,

    output  [63:0]  dout_ALU,
);

wire [127:0] tmp_dout_ALU;

always@(*)begin
    case({hloutalu,lenoutalu})
        00:dout_ALU = {32{tmp_dout_ALU[31]},tmp_dout_ALU[31:0]};
        01:dout_ALU = tmp_dout_ALU[63:0];
        11:dout_ALU = tmp_dout_ALU[127:64];
    endcase
end

always@(*)begin
    case(ALUCtrl)
        5'd0:tmp_dout_ALU = din1_ALU + din2_ALU;
        5'd1:tmp_dout_ALU = din1_ALU - din2_ALU;
        5'd2:tmp_dout_ALU = din1_ALU ^ din2_ALU;
        5'd3:tmp_dout_ALU = din1_ALU | din2_ALU;
        5'd4:tmp_dout_ALU = din1_ALU & din2_ALU;
        5'd5:tmp_dout_ALU = din1_ALU << din2_ALU;
        5'd6:tmp_dout_ALU = din1_ALU >> din2_ALU;
        5'd7:tmp_dout_ALU = din1_ALU >>> (din2_ALU[5:0]);
        5'd8:tmp_dout_ALU = din1_ALU == din2_ALU;
        5'd9:tmp_dout_ALU = din1_ALU != din2_ALU;
        5'd10:tmp_dout_ALU = $unsigned(din1_ALU) <  $unsigned(din2_ALU);
        5'd11:tmp_dout_ALU = $unsigned(din1_ALU) >= $unsigned(din2_ALU);
        //有符号比较din1<din2
        5'd12:tmp_dout_ALU = (din1_ALU[63]==0&&din2_ALU==0)?(din1<din2):
                                (din1_ALU[63]==1&&din2_ALU==1)?(din1<din2):
                                (din1_ALU[63]==0&&din2_ALU==1)?0:
                                (din1_ALU[63]==1&&din2_ALU==0)?1:64'bz;
        //有符号比较din1>=din2
        5'd13:tmp_dout_ALU = (din1_ALU[63]==0&&din2_ALU==0)?(din1>=din2):
                                (din1_ALU[63]==1&&din2_ALU==1)?(din1>=din2):
                                (din1_ALU[63]==0&&din2_ALU==1)?1:
                                (din1_ALU[63]==1&&din2_ALU==0)?0:64'bz;
        5'd14:tmp_dout_ALU[31:0] = din1_ALU[31:0] << din2_ALU;
        5'd15:tmp_dout_ALU[31:0] = din1_ALU[31:0] >> din2_ALU;
        5'd16:tmp_dout_ALU[31:0] = din1_ALU[31:0] >>> (din2_ALU[5:0]);
        5'd17:tmp_dout_ALU[31:0] = $signed(din1_ALU[31:0]) + $signed(din2_ALU[31:0]);//+wss
        5'd18:tmp_dout_ALU[31:0] = $signed(din1_ALU[31:0]) - $signed(din2_ALU[31:0]);//-wss
        5'd19:tmp_dout_ALU = $signed(din1_ALU[63:0])     * $signed(din[63:0]);//*ss
        5'd20:tmp_dout_ALU = $signed(din1_ALU[63:0])     * $unsigned(din[63:0]);//*su
        5'd21:tmp_dout_ALU = $unsigned(din1_ALU[63:0])   * $unsigned(din[63:0]);//*uu
        5'd22:tmp_dout_ALU = $signed(din1_ALU[31:0])     * $signed(din[31:0]);//*wss
        5'd23:tmp_dout_ALU = $signed(din1_ALU[63:0])     / $signed(din[63:0]);// /ss
        5'd24:tmp_dout_ALU = $unsigned(din1_ALU[63:0])   / $unsigned(din[63:0]);// /uu
        5'd25:tmp_dout_ALU = $signed(din1_ALU[31:0])     / $signed(din[31:0]);// /wss
        5'd26:tmp_dout_ALU = $unsigned(din1_ALU[31:0])   / $unsigned(din[31:0]);// /wuu
        5'd27:tmp_dout_ALU = $signed(din1_ALU[63:0])     % $signed(din[63:0]);// %ss
        5'd28:tmp_dout_ALU = $unsigned(din1_ALU[63:0])   % $unsigned(din[63:0]);// %uu
        5'd29:tmp_dout_ALU = $signed(din1_ALU[31:0])     % $signed(din[31:0]);// %wss
        5'd30:tmp_dout_ALU = $unsigned(din1_ALU[31:0])   % $unsigned(din[31:0]);// %wuu
        default:tmp_dout_ALU = 0;
    endcase
end
    
endmodule