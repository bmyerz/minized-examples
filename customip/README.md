Once you generate your IP top level file, edit it as follows

1. add this line to the top

```
`include "../src/Multiplier.v"
```

2. add these lines within the module

```
wire [31:0] multiply_out;
Multiplier mult_1(slv_reg0[31:16], slv_reg0[15:0], multiply_out);
```

3. change this line

```
2'h1   : reg_data_out <= slv_reg1;
```

to this

```
2'h1   : reg_data_out <= multiply_out;
```
          
    
