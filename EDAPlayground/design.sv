// --------------- sobel_filter.v ---------------
module sobel_filter (
    input clk,
    input reset,
    input [7:0] pixel_window[2:0][2:0], // 3x3 pixel window
    output reg [7:0] edge_output
);

    // Sobel filter coefficients explicitly defined
    parameter GX_00 = -1, GX_01 = 0, GX_02 = 1,
              GX_10 = -2, GX_11 = 0, GX_12 = 2,
              GX_20 = -1, GX_21 = 0, GX_22 = 1;

    parameter GY_00 = -1, GY_01 = -2, GY_02 = -1,
              GY_10 = 0,  GY_11 = 0,  GY_12 = 0,
              GY_20 = 1,  GY_21 = 2,  GY_22 = 1;

    integer gx, gy, result;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            edge_output <= 8'b0;
        end else begin
            gx = GX_00 * pixel_window[0][0] + GX_01 * pixel_window[0][1] + GX_02 * pixel_window[0][2] +
                 GX_10 * pixel_window[1][0] + GX_11 * pixel_window[1][1] + GX_12 * pixel_window[1][2] +
                 GX_20 * pixel_window[2][0] + GX_21 * pixel_window[2][1] + GX_22 * pixel_window[2][2];

            gy = GY_00 * pixel_window[0][0] + GY_01 * pixel_window[0][1] + GY_02 * pixel_window[0][2] +
                 GY_10 * pixel_window[1][0] + GY_11 * pixel_window[1][1] + GY_12 * pixel_window[1][2] +
                 GY_20 * pixel_window[2][0] + GY_21 * pixel_window[2][1] + GY_22 * pixel_window[2][2];

            result = gx*gx + gy*gy;
            if (result > 128) begin
                edge_output <= 8'b1;
            end else begin
                edge_output <= 8'b0;
            end
        end
    end

endmodule

// --------------- edge_detection.v ---------------
module edge_detection (
    input clk,
    input reset,
    input [7:0] pixel_input,
    input pixel_valid,
    output reg [7:0] edge_output,
    output reg edge_output_valid
);

    parameter IMG_WIDTH = 10; 
    parameter IMG_HEIGHT = 10; 

    reg [7:0] image_memory[IMG_WIDTH-1:0][IMG_HEIGHT-1:0];
    reg [7:0] pixel_window[2:0][2:0];

    reg [3:0] x = 0; // Changed to 4 bits to handle 0 to 9
    reg [3:0] y = 0; 

    sobel_filter sf (
        .clk(clk),
        .reset(reset),
        .pixel_window(pixel_window),
        .edge_output(edge_output)
    );

    integer i, j;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            x <= 0;
            y <= 0;
            edge_output_valid <= 0;
        end else if (pixel_valid) begin
            image_memory[x][y] <= pixel_input;

            x <= x + 1;
            if (x == IMG_WIDTH) begin
                x <= 0;
                y <= y + 1;
                if (y == IMG_HEIGHT) begin
                    y <= 0;
                end
            end

            if (x > 0 && x < IMG_WIDTH-1 && y > 0 && y < IMG_HEIGHT-1) begin
                for (i = 0; i < 3; i = i + 1) begin
                    for (j = 0; j < 3; j = j + 1) begin
                        pixel_window[i][j] <= image_memory[x+i-1][y+j-1];
                    end
                end
                edge_output_valid <= 1;
            end else begin
                edge_output_valid <= 0;
            end
        end
    end

endmodule