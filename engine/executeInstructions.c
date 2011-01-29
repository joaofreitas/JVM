/*
 * executeInstructions.c
 *
 *  Created on: 25/01/2011
 *      Author: joaofreitas
 */

#include "executeInstructions.h"
#include "../structures/frame.h"
#include "../structures/methodArea.h"
#include "executeEngine.h"

int wide=0;

u2 getParameterCount(char *method_descriptor) {
	u2 count;

	method_descriptor++;
	while (*method_descriptor != ')') {
		if (*method_descriptor == '[') {
			method_descriptor++;
		}
		if (*method_descriptor == 'L') {
			while (*method_descriptor != ';') {
				method_descriptor++;
			}
		}
		/*Double e long precisam ter 2 espaços*/
		if ((*method_descriptor == 'J') || (*method_descriptor == 'D')) {
			count++;
		}
		count++;
		method_descriptor++;
	}
	return count;
}


void func_aaload(){
	int index = popOperand();
	u4 array_aux = popOperand();
	u4 *arrayref;

	arrayref = malloc(sizeof(int));
	memcpy(&arrayref, &array_aux, sizeof(u4));

	if (arrayref == NULL){
		printf("\nNullPointerException at aaload.\n");
		return;
	}
	/*TODO como saber se index nao vai estourar arrayref?*/
	pushOperand(arrayref[index]);
}

void func_aastore(){

}

void func_aconst_null(){
}

void func_aload(){
	u1 index;
	u4 objectref;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if (wide) {
		frame_stack->frame->pc++;
		index = ((index << 8) | frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc]);
		wide = 0;
	}
	objectref = frame_stack->frame->local_variables[index];
	pushOperand(objectref);
}

void func_aload_0(){
	pushOperand(frame_stack->frame->local_variables[0]);
}

void func_aload_1(){
	pushOperand(frame_stack->frame->local_variables[1]);
}

void func_aload_2(){
	pushOperand(frame_stack->frame->local_variables[2]);
}

void func_aload_3(){
	pushOperand(frame_stack->frame->local_variables[3]);
}

void func_anewarray(){
}

void func_areturn(){
	u4 objectref = popOperand();

	popFrame(frame_stack->frame);
	pushOperand(objectref);
}

void func_arraylength(){
}

void func_astore(){
	u1 index;
	u4 objectref;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if (wide) {
		frame_stack->frame->pc++;
		index = ((index << 8) | frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc]);
		wide = 0;
	}
	objectref = popOperand();
	frame_stack->frame->local_variables[index] = objectref;

	/*TODO The aload instruction cannot be used to load a value
	 * of type returnAddress from a local variable onto the
	 * operand stack. This asymmetry with the astore instruction
	 * is intentional.*/
}

void func_astore_0(){
}

void func_astore_1(){
	u4 objectref = popOperand();

	frame_stack->frame->local_variables[1] = objectref;
}

void func_astore_2(){
}

void func_astore_3(){
	u4 objectref;

	objectref = popOperand();
	frame_stack->frame->local_variables[3] = objectref;
}

void func_athrow(){
}


void func_baload(){
	int index = popOperand();
	u4 array_aux = popOperand();
	char *arrayref;

	arrayref = malloc(sizeof(u1));
	memcpy(&arrayref, &array_aux, sizeof(u4));

	if (arrayref == NULL){
		printf("\nNullPointerException at baload.\n");
		return;
	}
	/*TODO como saber se index nao vai estourar arrayref?*/
	pushOperand((signed) arrayref[index]);
}

void func_bastore(){
}

void func_bipush(){
	int value;

	frame_stack->frame->pc++;
	value = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if(value & 0x00000080) {
		value |= 0xffffff00;
	}
	else {
		value &= 0x000000ff;
	}
	pushOperand(value);
}


void func_caload(){
}

void func_castore(){

}

void func_checkcast(){
}


void func_d2f(){
	double d_value;
	float f_value;
	u4 high_bytes, low_bytes;
	u4 stackValue;

	high_bytes = popOperand();
	low_bytes = popOperand();
	d_value = getDouble(low_bytes, high_bytes);
	f_value = (float)d_value;
	memcpy(&stackValue, &f_value, sizeof(float));

	pushOperand(stackValue);
}

void func_d2i(){
}

void func_d2l(){
	double d_value;
	long long l_value;
	u4 high_bytes, low_bytes;

	high_bytes = popOperand();
	low_bytes = popOperand();
	d_value = getDouble(low_bytes, high_bytes);

	l_value = (long long)d_value;

	pushOperand(getLongLowBytes(l_value));
	pushOperand(getLongHighBytes(l_value));
}

void func_dadd(){
}

void func_daload(){
	u4 index;
	arrays_t *arrayref;
	double *ref;
	u4 low_bytes, high_bytes;

	index = popOperand();
	arrayref = popOperand();
	if (arrayref == NULL){
		printf("\nNullPointerException at faload.\n");
		return;
	}
	if (index >= arrayref->size || index < 0){
		printf("\nArrayIndexOutOfBoundsException at faload.\n");
		return;
	}

	ref = (double *) arrayref->reference;

	low_bytes = getDoubleLowBytes(ref[index]);
	high_bytes = getDoubleHighBytes(ref[index]);

	pushOperand(low_bytes);
	pushOperand(high_bytes);
}

void func_dastore(){
}

void func_dcmpg(){
}

void func_dcmpl(){
}

void func_dconst_0(){
	u4 low_bytes = 0;
	u4 high_bytes = 0;
	popOperand(low_bytes);
	popOperand(high_bytes);
}

void func_dconst_1(){
}

void func_ddiv(){
}

void func_dload(){
}

void func_dload_0(){
	u4 low_bytes, high_bytes;

	high_bytes = frame_stack->frame->local_variables[0];
	low_bytes = frame_stack->frame->local_variables[1];

	pushOperand(low_bytes);
	pushOperand(high_bytes);
}

void func_dload_1(){
}

void func_dload_2(){
}

void func_dload_3(){
	u4 low_bytes, high_bytes;

	high_bytes = frame_stack->frame->local_variables[3];
	low_bytes = frame_stack->frame->local_variables[4];

	pushOperand(low_bytes);
	pushOperand(high_bytes);
}

void func_dmul(){
	u4 low_bytes1, high_bytes1;
	u4 low_bytes2, high_bytes2;
	u4 result_low_bytes, result_high_bytes;
	double value1, value2, result;

	high_bytes1 = popOperand();
	low_bytes1 = popOperand();
	high_bytes2 = popOperand();
	low_bytes2 = popOperand();

	value1 = getDouble(low_bytes1, high_bytes1);
	value2 = getDouble(low_bytes2, high_bytes2);

	result = value1*value2;
	result_high_bytes = getDoubleHighBytes(result);
	result_low_bytes = getDoubleLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_dneg(){
}

void func_drem(){
}

void func_dreturn(){
}

void func_dstore(){
	u4 index;
	u4 low_bytes, high_bytes;

	high_bytes = popOperand();
	low_bytes = popOperand();

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if (wide) {
		frame_stack->frame->pc++;
		index = ((index << 8) | frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc]);
		wide = 0;
	}

	frame_stack->frame->local_variables[index] = high_bytes;
	frame_stack->frame->local_variables[index+1] = low_bytes;
}

void func_dstore_0(){
}

void func_dstore_1(){
}

void func_dstore_2(){
}

void func_dstore_3(){
	u4 low_bytes, high_bytes;

	high_bytes = popOperand();
	low_bytes = popOperand();
	frame_stack->frame->local_variables[3] = high_bytes;
	frame_stack->frame->local_variables[4] = low_bytes;
}

void func_dsub(){
}

void func_dup(){
}

void func_dup_x1(){
}

void func_dup_x2(){
	u4 value1, value2, value3;

	value1 = popOperand();
	value2 = popOperand();
	value3 = popOperand();

	pushOperand(value1);
	pushOperand(value3);
	pushOperand(value2);
	pushOperand(value1);
}

void func_dup2(){
}

void func_dup2_x1(){
}

void func_dup2_x2(){
}


void func_f2d(){
	float value;
	double result;
	u4 operand;
	u4 low_bytes, high_bytes;

	operand = popOperand();
	memcpy(&value, &operand, sizeof(float));
	result = (double)value;
	low_bytes = getDoubleLowBytes(result);
	high_bytes = getDoubleHighBytes(result);

	pushOperand(low_bytes);
	pushOperand(high_bytes);
}

void func_f2i(){
}

void func_f2l(){
}

void func_fadd(){
}

void func_faload(){
	u4 index;
	arrays_t *arrayref;
	float *ref;
	u4 stackValue;

	index = popOperand();
	*arrayref = popOperand();
	if (arrayref == NULL){
		printf("\nNullPointerException at faload.\n");
		return;
	}
	if (index >= arrayref->size || index < 0){
		printf("\ArrayIndexOutOfBoundsException at faload.\n");
		return;
	}

	ref = (float *) arrayref->reference;
	memcpy(&stackValue, &ref[index], sizeof(u4));

	pushOperand(stackValue);
}

void func_fastore(){
}

void func_fcmpg(){
}

void func_fcmpl(){
}

void func_fconst_0(){

}

void func_fconst_1(){
}

void func_fconst_2(){
}

void func_fdiv(){

}

void func_fload(){
	u4 index, localValue;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if (wide) {
		frame_stack->frame->pc++;
		index = ((index << 8) | frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc]);
		wide = 0;
	}
	localValue = frame_stack->frame->local_variables[index];
	pushOperand(localValue);
}

void func_fload_0(){
}

void func_fload_1(){
}

void func_fload_2(){
}

void func_fload_3(){
}

void func_fmul(){
}

void func_fneg(){
}

void func_frem(){
}

void func_freturn(){
	u4 value;

	value = popOperand();
	popFrame();
	pushOperand(value);
}

void func_fstore(){
}

void func_fstore_0(){
}

void func_fstore_1(){
}

void func_fstore_2(){

}

void func_fstore_3(){
}

void func_fsub(){
}


void func_getfield(){
}

void func_getstatic(){
	u4 indexbyte1, indexbyte2;
	u4 index;
	u2 value;
	class *cl;
	cp_info field;
	field_info *fieldref;
	char *class_name;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;

	field = getConstantPoolElementByIndexFromCurrentFrame(index);
	class_name = getConstanPoolElement(field.constant_union.c_class.name_index).constant_union.c_utf8.bytes;
	cl = getSymbolicReferenceClass(class_name);

	fieldref = getResolvedFieldReference(cl, field);

	if(fieldref == NULL) {
		return;
	}

	/*value = getConstantPoolElementByIndexFromCurrentFrame(fieldref->attributes.attribute_union.constant_value.constant_value_index);
	pushOperand();*/
}

void func_goto(){
}

void func_goto_w(){
}


void func_i2b(){
}

void func_i2c(){
	u4 value;

	value = (u1)popOperand();
	value &= 0x000000ff;
	pushOperand(value);
}

void func_i2d(){
}

void func_i2f(){
}

void func_i2l(){
}

void func_i2s(){
	u4 value;

	value = (u2)popOperand();
	value &= 0x0000ffff;
	pushOperand(value);
}

void func_iadd(){
	int op1 = popOperand();
	int op2 = popOperand();
	int result;

	result = op1+op2;

	pushOperand(result);
}

void func_iaload(){
}

void func_iand(){
}

void func_iastore(){
	u4 index;
	arrays_t *arrayref;
	int *ref;
	u4 stackValue;

	stackValue = popOperand();
	index = popOperand();
	*arrayref = popOperand();

	if (arrayref == NULL){
		printf("\nNullPointerException at iastore.\n");
		return;
	}
	if (index >= arrayref->size || index < 0){
		printf("\ArrayIndexOutOfBoundsException at iastore.\n");
		return;
	}
	arrayref->reference[index] = stackValue;
}

void func_iconst_m1(){
}

void func_iconst_0(){
}

void func_iconst_1(){
	pushOperand(1);
}

void func_iconst_2(){
	pushOperand(2);
}

void func_iconst_3(){
	pushOperand(3);
}

void func_iconst_4(){
	pushOperand(4);
}

void func_iconst_5(){
	pushOperand(5);
}

void func_idiv(){
	int value1, value2;

	value2 = popOperand();
	value1 = popOperand();

	if (value2 == 0) {
		printf("\ArithmeticException at idiv.\n");
	}

	pushOperand((u4)value1/value2);
}

void func_if_acmpeq(){
}

void func_if_acmpne(){
}

void func_if_icmpeq(){
}

void func_if_icmpne(){
	int value1, value2;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value2 = popOperand();
	value1 = popOperand();

	if(value1 != value2) {
		frame_stack->frame->pc++;
		brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
			/*TODO offset -1? */
		}
	}
}

void func_if_icmplt(){
}

void func_if_icmpge(){
}

void func_if_icmpgt(){
}

void func_if_icmple(){
	int value1, value2;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value2 = popOperand();
	value1 = popOperand();

	if(value1 <= value2) {
		frame_stack->frame->pc++;
		brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
			/*TODO offset -1? */
		}
	}
}

void func_ifeq(){
}

void func_ifne(){
}

void func_iflt(){
}

void func_ifge(){
	int value;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value = popOperand();

	if(value >= 0) {
		frame_stack->frame->pc++;
		brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
		}
	}
}

void func_ifgt(){
}

void func_ifle(){
}

void func_ifnonnull(){
}

void func_ifnull(){
	u4 *value;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	*value = popOperand();

	if(value == NULL) {
		frame_stack->frame->pc++;
		brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
		}
	}
}

void func_iinc(){
}

void func_iload(){
	u2 index, index2;
	int value;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if (wide){
		frame_stack->frame->pc++;
		index2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		index = ((index << 8)|index2);
		wide = 0;
	}

	value = frame_stack->frame->local_variables[index];

	pushOperand(value);
}

void func_iload_0(){
	int value;
	value = frame_stack->frame->local_variables[0];
	pushOperand(value);
}

void func_iload_1(){
	int value;
	 value = frame_stack->frame->local_variables[1];
	 pushOperand(value);
}

void func_iload_2(){
	int value;
	 value = frame_stack->frame->local_variables[2];
	 pushOperand(value);
}

void func_iload_3(){
	int value;
	 value = frame_stack->frame->local_variables[3];
	 pushOperand(value);
}

void func_imul(){
}

void func_ineg(){
	int value;

	value = popOperand();
	pushOperand((~value)+1);
}

void func_instanceof(){
}

void func_invokeinterface(){
}

void func_invokespecial(){
	func_invokevirtual();
}

void func_invokestatic(){
	unsigned char indexbyte1;
	unsigned char indexbyte2;
	char *class_name, *method_name, *method_descriptor;
	cp_info method_ref_info, class_info, method_name_type_ref_info;
	method_info *invoke_method;
	frame_t *frame;
	u4 operand,i;
	u2 index, parameter_count;
	class *resolved_class;

	frame_stack->frame->pc++;
	indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

	method_ref_info = getConstanPoolElement(index);

	/*Pegando o nome da classe*/
	class_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.class_index);
	class_name = (char *)getConstanPoolElement(class_info.constant_union.c_class.name_index).constant_union.c_utf8.bytes;

	/*Pegando o nome e o descritor do método*/
	method_name_type_ref_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.name_and_type_index);
	method_name = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.name_index).constant_union.c_utf8.bytes;
	method_descriptor = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.descriptor_index).constant_union.c_utf8.bytes;

	resolved_class = getSymbolicReferenceClass(class_name);
	if (resolved_class == NULL) {
		printf("ClassNotFoundException!\n");
		exit(0);
	}

	/*Metodo a ser invocado*/
	invoke_method = getMethod(resolved_class->class_file, method_name, method_descriptor);
	if (invoke_method == NULL) {
		printf("MethodNotFoundException!\n");
		exit(0);
	}

	parameter_count = getParameterCount(method_descriptor);

	frame = createFrame(invoke_method, resolved_class->class_file->constant_pool, 0);

	/*Devo empilhar ao contrário, pois a pilha vai conter a object reference por ultimo.
	Comeca com 1 por causa do indice do vetor*/
	for (i=1; i <= parameter_count; i++) {
		operand = popOperand();
		frame->local_variables[parameter_count-i] = operand;
	}

	pushFrame(frame);
}

void func_invokevirtual(){

	unsigned char indexbyte1;
	unsigned char indexbyte2;
	char *class_name, *method_name, *method_descriptor;
	cp_info method_ref_info, class_info, method_name_type_ref_info;
	method_info *invoke_method;
	frame_t *frame;
	u4 operand,i;
	u2 index, parameter_count;
	class *resolved_class;

	frame_stack->frame->pc++;
	indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

	method_ref_info = getConstanPoolElement(index);

	/*Pegando o nome da classe*/
	class_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.class_index);
	class_name = (char *)getConstanPoolElement(class_info.constant_union.c_class.name_index).constant_union.c_utf8.bytes;

	/*Pegando o nome e o descritor do método*/
	method_name_type_ref_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.name_and_type_index);
	method_name = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.name_index).constant_union.c_utf8.bytes;
	method_descriptor = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.descriptor_index).constant_union.c_utf8.bytes;

	resolved_class = getSymbolicReferenceClass(class_name);
	if (resolved_class == NULL) {
		printf("ClassNotFoundException!\n");
		exit(0);
	}

	/*Metodo a ser invocado*/
	invoke_method = getMethod(resolved_class->class_file, method_name, method_descriptor);
	if (invoke_method == NULL) {
		printf("MethodNotFoundException!\n");
		exit(0);
	}

	parameter_count = getParameterCount(method_descriptor);

	frame = createFrame(invoke_method, resolved_class->class_file->constant_pool, 0);

	/*Devo empilhar ao contrário, pois a pilha vai conter a object reference por ultimo.
	Comeca com 1 por causa do indice do vetor*/
	for (i=1; i <= parameter_count; i++) {
		operand = popOperand();
		frame->local_variables[parameter_count-i] = operand;
	}

	pushFrame(frame);

}

void func_ior(){
}

void func_irem(){
}

void func_ireturn(){
	u4 value;

	value = popOperand();
	popFrame();
	pushOperand(value);
}

void func_ishl(){
}

void func_ishr(){
}

void func_istore(){
	int value = popOperand();
	u2 index;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->local_variables[index] = value;
}

void func_istore_0(){
	int value = popOperand();
	frame_stack->frame->local_variables[0] = value;
}

void func_istore_1(){
	int value = popOperand();
	frame_stack->frame->local_variables[1] = value;
}

void func_istore_2(){
	int value = popOperand();
	frame_stack->frame->local_variables[2] = value;
}

void func_istore_3(){
	int value = popOperand();
	frame_stack->frame->local_variables[3] = value;
}

void func_isub(){
	int value1, value2;

	value2 = popOperand();
	value1 = popOperand();

	pushOperand((u4)value1-value2);
}

void func_iushr(){
}

void func_ixor(){
}


void func_jsr(){
}

void func_jsr_w(){
	u4 branchbyte1, branchbyte2, branchbyte3, branchbyte4;
	u4 offset;

	frame_stack->frame->pc++;
	branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte3 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte4 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	pushOperand((u4)frame_stack->frame->pc+1);
	offset = (branchbyte1 << 24) | (branchbyte2 << 16) | (branchbyte3 << 8) | branchbyte4;
	frame_stack->frame->pc += offset-1;
}


void func_l2d(){
}

void func_l2f(){
}

void func_l2i(){
}

void func_ladd(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);
	result = value1+value2;
	result_high_bytes = getLongHighBytes(result);
	result_low_bytes = getLongLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_laload(){
}

void func_land(){
}

void func_lastore(){
}

void func_lcmp(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);

	if (value1 > value2) {
		pushOperand(1);
	}
	else if(value1 == value2) {
		pushOperand(0);
	}
	else {
		pushOperand(-1);
	}
}

void func_lconst_0(){
}

void func_lconst_1(){
}

void func_ldc(){
}

void func_ldc_w(){
	u4 indexbyte1, indexbyte2;
	u4 index;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;


	switch(frame_stack->frame->cp[index].tag)
	{
		case 3:/*CONSTANT_Integer:*/
		{
			pushOperand(frame_stack->frame->cp[index].constant_union.c_integer.bytes);
		}
		break;
		case 4:/*CONSTANT_Float:*/
		{
			pushOperand(frame_stack->frame->cp[index].constant_union.c_float.bytes);
		}
		break;
		case 8:/*CONSTANT_String:*/
		{
			pushOperand(frame_stack->frame->cp[index].constant_union.c_string.string_index);
		}
		break;
		default:
			printf("\n\ldc_w ERROR\n");
		break;
	}
}

void func_ldc2_w(){
	u4 indexbyte1, indexbyte2;
		u4 index;

		frame_stack->frame->pc++;
		indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

		index = (indexbyte1 << 8) | indexbyte2;


		switch(frame_stack->frame->cp[index].tag)
		{
			case 3:/*CONSTANT_Integer:*/
			{
				pushOperand(frame_stack->frame->cp[index].constant_union.c_integer.bytes);
			}
			break;
			case 4:/*CONSTANT_Float:*/
			{
				pushOperand(frame_stack->frame->cp[index].constant_union.c_float.bytes);
			}
			break;
			case 8:/*CONSTANT_String:*/
			{
				pushOperand(frame_stack->frame->cp[index].constant_union.c_string.string_index);
			}
			break;
			default:
				printf("\n\ldc_w ERROR\n");
			break;
		}
}

void func_ldiv(){
}

void func_lload(){
}

void func_lload_0(){

}

void func_lload_1(){
}

void func_lload_2(){
}

void func_lload_3(){
}

void func_lmul(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);
	result = value1*value2;
	result_high_bytes = getLongHighBytes(result);
	result_low_bytes = getLongLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_lneg(){
}

void func_lookupswitch(){
}

void func_lor(){
}

void func_lrem(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);
	if (value2 == 0) {
		printf("\ArithmeticException at idiv.\n");
	}

	result = value1 - (value1 / value2) * value2;

	result_high_bytes = getLongHighBytes(result);
	result_low_bytes = getLongLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_lreturn(){
	u4 low_bytes, high_bytes;

	high_bytes = popOperand();
	low_bytes = popOperand();

	popFrame();
	pushOperand(low_bytes);
	pushOperand(high_bytes);
}

void func_lshl(){
}

void func_lshr(){
}

void func_lstore(){
	u8 value, result;
	u4 high_bytes, low_bytes;
	u4 index;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	high_bytes = popOperand();
	low_bytes = popOperand();

	frame_stack->frame->local_variables[index] = high_bytes;
	frame_stack->frame->local_variables[index+1] = low_bytes;
}

void func_lstore_0(){
}

void func_lstore_1(){
}

void func_lstore_2(){
}

void func_lstore_3(){
}

void func_lsub(){
}

void func_lushr(){
}

void func_lxor(){
}


void func_monitorenter(){
	/*TODO chapolin*/
}

void func_monitorexit(){
}

void func_multianewarray(){
}


void func_new(){
}

void func_newarray(){
	/*TODO chapolin*/
}

void func_nop(){
}


void func_pop(){
}

void func_pop2(){
}

void func_putfield(){
	u4 indexbyte1, indexbyte2;
	u4 index;
	u4 *objectref;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;
	/*TODO chapolin*/
}

void func_putstatic(){
}


void func_ret(){

}

void func_return(){
	popFrame();
}


void func_saload(){
	u4 index;
	arrays_t *arrayref;
	short *ref;
	u4 stackValue;

	index = popOperand();
	arrayref = (arrays_t*)popOperand();
	if (arrayref == NULL){
		printf("\nNullPointerException at faload.\n");
		return;
	}
	if (index >= arrayref->size || index < 0){
		printf("\ArrayIndexOutOfBoundsException at faload.\n");
		return;
	}

	ref = (short *) arrayref->reference;
	memcpy(&stackValue, &ref[index], sizeof(short));

	pushOperand(stackValue);
}

void func_sastore(){
}

void func_sipush(){
}

void func_swap(){
}


void func_tableswitch(){
	/*TODO chapolin*/
}


void func_wide(){
	wide=1;
}


void func_breakpoint(){
}

void func_impdep1(){
}

void func_impdep2(){
}

