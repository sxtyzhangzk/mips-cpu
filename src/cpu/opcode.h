`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2017/06/18 20:51:37
// Design Name: 
// Module Name: opcode
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

`define ALU_OPCODE_WIDTH 5

`define ALU_NOP		0
`define ALU_ADD		1
`define ALU_ADDU	2
`define ALU_SUB		3
`define ALU_SUBU	4
`define ALU_MULTL	5
`define ALU_MULTH	6
`define ALU_MULTLU	7
`define ALU_MULTHU	8
`define ALU_DIV		9
`define ALU_MOD		10
`define ALU_DIVU	11
`define ALU_MODU	12
`define ALU_AND		13
`define ALU_OR		14
`define ALU_NOR		15
`define ALU_XOR		16
`define ALU_SLL		17
`define ALU_SRL		18
`define ALU_SRA		19
`define ALU_ROR		20
`define ALU_SEQ		21
`define ALU_SLT		22
`define ALU_SLTU	23

`define MEM_OPCODE_WIDTH 4

`define MEM_NOP		0
`define MEM_LB		1
`define MEM_LBU		2
`define MEM_LH		3
`define MEM_LHU		4
`define MEM_LW		5
`define MEM_SB		6
`define MEM_SH		7
`define MEM_SW		8

