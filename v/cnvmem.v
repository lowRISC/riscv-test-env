module cnvmem;

   integer i, fd, first, last;

   reg [7:0] mem[32'h00000000:32'h00100000];
   reg [127:0] mem2[0:'hfff];
   reg [63:0] mem3[0:'h1fff];

   initial
     begin
        $readmemh("cnvmem.mem", mem);
        for (i = 32'h00000000; (i < 32'h00100000) && (1'bx === ^mem[i]); i=i+16)
          ;
        first = i;
        for (i = 32'h00100000; (i >= 32'h00000000) && (1'bx === ^mem[i]); i=i-16)
          ;
        last = (i+16);
        if (last < first + 'H10000)
             last = first + 'H10000;
        for (i = i+1; i < last; i=i+1)
          mem[i] = 0;
        $display("First = %X, Last = %X", first, last-1);
        for (i = first; i < last; i=i+1)
          if (1'bx === ^mem[i]) mem[i] = 0;
        
        for (i = first; i < last; i=i+16)
          begin
             mem2[(i/16)&'hFFF] = {mem[i+15],mem[i+14],mem[i+13],mem[i+12],
                                   mem[i+11],mem[i+10],mem[i+9],mem[i+8],
                                   mem[i+7],mem[i+6],mem[i+5],mem[i+4],
                                   mem[i+3],mem[i+2],mem[i+1],mem[i+0]};
          end      
        for (i = first; i < last; i=i+8)
          begin
             mem3[(i/8)&'h1FFF] = {mem[i+7],mem[i+6],mem[i+5],mem[i+4],
                                   mem[i+3],mem[i+2],mem[i+1],mem[i+0]};
          end
        fd = $fopen("cnvmem.hex", "w");
        for (i = 0; i <= 'hfff; i=i+1)
          $fdisplay(fd, "%32x", mem2[i]);
        $fclose(fd);
        fd = $fopen("cnvmem64.hex", "w");
        for (i = 0; i <= 'h1fff; i=i+1)
          $fdisplay(fd, "%16x", mem3[i]);
        $fclose(fd);
        fd = $fopen("cnvmem.coe", "w");
        $fdisplay(fd, "memory_initialization_radix=16;");
        $fdisplay(fd, "memory_initialization_vector=");
        for (i = 0; i <= 'hfff; i=i+1)
          $fdisplay(fd, "%32x", mem2[i]);
        $fclose(fd);
     end
   
endmodule // cnvmem
