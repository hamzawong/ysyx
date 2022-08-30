module ROM (
    input   [19:0]  ain_ROM,
    output  [31:0]  dout_ROM
);

reg [7:0]   rom[19:0];

assign dout_ROM = {rom[ain_ROM+3],rom[ain_ROM+2],rom[ain_ROM+1],rom[ain_ROM]};
    
endmodule