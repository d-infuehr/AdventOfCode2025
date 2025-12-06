import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class main {
    public static void main(String[] args) {
        String[][] data = new String[0][0];
        try {
            data = Files.lines(Paths.get("input.txt"))
                    .map(line -> line.trim().split("\\s+"))
                    .toArray(String[][]::new);
                    
        } catch (IOException e) {
            System.err.println("Error reading input.txt");
            return; 
        }
            
        int rows = data.length;
        int cols = data[0].length;
        
        //Offbrand linq: Take(length-1).Select(Long.Parse).ToArray()
        long[][] input = Arrays.stream(data).limit(rows - 1)
                .map(row -> Arrays.stream(row).mapToLong(Long::parseLong).toArray())
                .toArray(long[][]::new);  
        
        long total = 0;
        for(int col=0;col<cols;col++) {
            String OP = data[rows - 1][col];
            boolean mult = OP.charAt(0) == '*';
            long unit = mult ? 1 : 0;
            for(int row = 0; row < rows - 1; row++)
            {
                if (mult) unit *= input[row][col];
                else unit += input[row][col];
            }
            total += unit;
        }
        System.out.println("TOTAL Part 1: " + total);
        total = 0;

        char[][] grid = new char[0][0];
        int gridwidth = 0;
        try {
            List<String> lines = Files.readAllLines(Path.of("input.txt"));
            grid = new char[lines.size()][];
            for (int i = 0; i < lines.size(); i++) {
                grid[i] = lines.get(i).toCharArray();
                gridwidth = Math.max(gridwidth, grid[i].length);
            }
        } catch (IOException e) {
            System.err.println("Error reading input.txt");
            return; 
        }
        int gridheight = grid.length;
        
        List<Integer> seperators = new ArrayList<>();
        seperators.add(0);
        for(int i = 0; i < gridwidth; i++){
            int m = 0;
            for(int h = 0; h < gridheight; h++){
                if (grid[h][i] == ' ') m++;
            }
            if (m == gridheight) seperators.add(i + 1);
        }
        seperators.add(gridwidth + 1);

        for(int i = 0; i < seperators.size() - 1; i++) {
            int start = seperators.get(i);
            int end = seperators.get(i + 1) - 1;
            int height = gridheight - 1;
            char OP = grid[height][start];
            boolean mult = OP == '*';
            long unit = mult ? 1 : 0;
            for(int w = start; w < end; w++) {
                StringBuilder nrStr = new StringBuilder();
                for(int h = 0; h < height; h++){
                    char c = grid[h][w];
                    if (c != ' ') nrStr.append(c);
                }
                long nr = Long.parseLong(nrStr.toString());
                if (mult) unit *= nr;
                else unit += nr;
             }
            total += unit;
        }
        System.out.println("TOTAL Part 2: " + total);
    }
}