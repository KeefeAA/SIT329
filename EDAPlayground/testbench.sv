module tb_edge_detection;

    reg clk;
    reg reset;
    reg [7:0] pixel_input[9:0][9:0];
    reg pixel_valid;
    wire [7:0] edge_output;
    wire edge_output_valid;

    // Define x and y for pixel tracking
    reg [3:0] x = 0;
    reg [3:0] y = 0;

    // Instantiate the edge_detection module
    edge_detection uut (
        .clk(clk),
        .reset(reset),
        .pixel_input(pixel_input[x][y]),
        .pixel_valid(pixel_valid),
        .edge_output(edge_output),
        .edge_output_valid(edge_output_valid)
    );

    // Clock generation
    always begin
        #5 clk = ~clk;
    end

    // Test sequence
    initial begin
        clk = 0;
        reset = 1;
        pixel_valid = 0;

        // Initialization
        #10 reset = 0;

        // Feed in a 10x10 image with the first half as 8'h00 and the other half as 8'hFF
        for (x = 0; x < 10; x = x + 1) begin
            for (y = 0; y < 10; y = y + 1) begin
                if(x < 5) 
                    pixel_input[x][y] = 8'h00;
                else
                    pixel_input[x][y] = 8'hFF;
                
                pixel_valid = 1;
                #10;
                pixel_valid = 0;
                #10;
            end
        end

        $finish;  // Properly terminate the simulation after the test
    end

    // Debug outputs
    always @(posedge clk) begin
        if (pixel_valid && x == 4 && y == 5) begin
            $display("Pixel input at (%d,%d): %02h", x, y, pixel_input[x][y]);
        end

        if (edge_output_valid && edge_output != 8'h00) begin
            $display("Edge Output at (%d,%d): %02h", x, y, edge_output);
        end
    end


endmodule