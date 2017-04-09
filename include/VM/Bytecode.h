#ifndef SPACE_BYTECODE_H
#define SPACE_BYTECODE_H

namespace VM {

	enum ByteInst {
		NOP =		0x00,
		MOV_RN =	0x01,
		MOV_RR =	0x02,

		ADD_RN =	0x04,
		ADD_RR = 	0x05,

		SUB_RN =	0x07,
		SUB_RR = 	0x08,

		CALL_ = 	0x0A,
		RET_ =		0x0B,
		SYSI_ = 	0x0C,

		MUL_RN =	0x10,
		MUL_RR =	0x11,

		DIV_RN = 	0x13,
		DIV_RR = 	0x14,

		SXR_R = 	0x16,
		SXL_R = 	0x17,
		INC_R = 	0x18,
		DEC_R = 	0x19,

		PUSH_R = 	0x1A,
		PUSH_N = 	0x1B,
		POP_R = 	0x1C,
		POP_X = 	0x1D,
		
		CMP_RR = 	0x1E,
		CMP_RN = 	0x1F,
		JMP_ = 		0x20,
		JNE_ = 		0x21,
		JE_ = 		0x22,
		JG_ = 		0x23,
		JL_ = 		0x24,
		JGE_ = 		0x25,
		JLE_ = 		0x26,
		JZ_ = 		0x27,
		JNZ_ = 		0x28,
		
		AND_RN = 	0x30,
		AND_RR = 	0x31,
		OR_RN = 	0x32,
		OR_RR =		0x33,
		XOR_RN = 	0x34,
		XOR_RR = 	0x35,

		HLT_ = 		0x40,
	};

	enum ByteReg {
		AX_ = 0x00,
		BX_ = 0x01,
		CX_ = 0x02,
		DX_ = 0x03,
		XX_ = 0x04,
		YX_ = 0x05,

		AL_ = 0x10,
		BL_ = 0x11,
		CL_ = 0x12,
		DL_ = 0x13,
		XL_ = 0x14,
		YL_ = 0x15,

		AH_ = 0x20,
		BH_ = 0x21,
		CH_ = 0x22,
		DH_ = 0x23,
		XH_ = 0x24,
		YH_ = 0x25,

		CF_ = 0x30,
		CY_ = 0x31,
		BP_ = 0x32,
		SP_ = 0x33,
		RM_ = 0x34,
		PC_ = 0x35,
	};

} // namespace VM

#endif // SPACE_BYTECODE_H