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
	pushOperand(NULL);
}

void func_aload(){
	u1 index;
	u4 objectref;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if (wide) {
		frame_stack->frame->pc++;
		index = ((index << 8) | frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc]);
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
	u4 objectref = popOperand();

	frame_stack->frame->local_variables[2] = objectref;
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
	int value;
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
	int value = popOperand();
	int index = popOperand();
	char *arrayref;
	u4 arrayaux;

	arrayref = malloc(sizeof(char));
	memcpy(&arrayref, &array_aux, sizeof(u4));

	if (arrayref == NULL){
		printf("\nNullPointerException at caload.\n");
		return;
	}
	/*TODO como saber se index nao vai estourar arrayref?*/
	arrayref[index]=(char *)value;

}

void func_checkcast(){
	u4 objectref = popOperand();
	u1 indexbyte1, indexbyte2;
	u2 index;
	cp_info resolved_class_type;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = ((indexbyte1 << 8) | indexbyte2);

	resolved_class_type = getConstantPoolElementByIndex(index);
	/*TODO se der tempo*/


}


void func_d2f(){
	double d_value;
	float f_value;

	u4 value1, value2;
	value1 = popOperand();
	value2 = popOperand();

	d_value = value1;
	d_value = ((d_value << 32) | value2);
	f_value = (float)d_value;
	pushOperand(f_value);
}

void func_d2i(){
}

void func_d2l(){
	u4 high = popOperand();
	u4 low = popOperand();
	double value;
	long result;

	result |= high;
	result = result << 32;
	result |= low;

	result = (long *)value;
	/*TODO verificar NaN e se esse cast resolve*/

	pushOperand(result);
}

void func_dadd(){
	u4 op1_hi = popOperand();
	u4 op1_low = popOperand();
	u4 op2_hi = popOperand();
	u4 op2_low = popOperand();
	u4, result_low, result_high;
	double op1, op2, result;
	u8 op_aux, result_aux;

	op_aux |= op1_hi;
	op_aux = op_aux << 32;
	op_aux |= op1_low;
	memcpy(&op1, &op_aux, sizeof(u8));

	op_aux |= op2_hi;
	op_aux = op_aux << 32;
	op_aux |= op2_low;
	memcpy(&op2, &op_aux, sizeof(u8));

	result = op1+op2;
	memcpy(&result_aux, &result, sizeof(u8));
	result_low |= result_aux;
	result_aux = result_aux >> 32;
	result_high |= result_aux;

	pushOperand(result_low);
	pushOperand(result_high);
}

void func_daload(){

}

void func_dastore(){
}

void func_dcmpg(){
}

void func_dcmpl(){
	u4 op1_hi = popOperand();
	u4 op1_low = popOperand();
	u4 op2_hi = popOperand();
	u4 op2_low = popOperand();
	int result;
	u8 op_aux;
	double op1, op2;

	op_aux |= op1_hi;
	op_aux = op_aux << 32;
	op_aux |= op1_low;
	memcpy(&op1, &op_aux, sizeof(u8));

	op_aux |= op2_hi;
	op_aux = op_aux << 32;
	op_aux |= op2_low;
	memcpy(&op2, &op_aux, sizeof(u8));

	if (op2 > op1){
		result = 1;
	}
	else if (op2 == op1){
		result = 0;
	}
	else
		result = -1;

	pushOperand(result);

}


void func_dconst_0(){
}

void func_dconst_1(){
}

void func_ddiv(){
}

void func_dload(){
	short index, index2;
	u4 value_hi, value_low;

	frame_stack->frame->pc++;

	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (wide){
		frame_stack->frame->pc++;
		index2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		index = ((index << 8)|index2);
		wide = 0;
	}

	value_hi = frame_stack->frame->local_variables[index];
	value_low = frame_stack->frame->local_variables[index+1];

	pushOperand(value_low);
	pushOperand(value_hi);
}

void func_dload_0(){



}

void func_dload_1(){
}

void func_dload_2(){
}

void func_dload_3(){
}

void func_dmul(){
}

void func_dneg(){
}

void func_drem(){
}

void func_dreturn(){
	u4 value_hi = popOperand();
	u4 value_low = popOperand();

	popFrame();

	pushOperand(value_low);
	pushOperand(value_hi);
}

void func_dstore(){

}

void func_dstore_0(){
	frame_stack->frame->local_variables[0] = popOperand();
	frame_stack->frame->local_variables[1] = popOperand();
}

void func_dstore_1(){
}

void func_dstore_2(){
	frame_stack->frame->local_variables[2] = popOperand();
	frame_stack->frame->local_variables[3] = popOperand();
}

void func_dstore_3(){
	frame_stack->frame->local_variables[3] = popOperand();
	frame_stack->frame->local_variables[4] = popOperand();
}

void func_dsub(){
}

void func_dup(){
}

void func_dup_x1(){
	u4 value1 = popFrame();
	u4 value2 = popFrame();

	pushOperand(value1);
	pushOperand(value2);
	pushOperand(value1);
}

void func_dup_x2(){
}

void func_dup2(){
}

void func_dup2_x1(){
}

void func_dup2_x2(){
	u4 value1 = popFrame();
	u4 value2 = popFrame();
	u4 value3 = popFrame();
	u4 value4 = popFrame();

	pushOperand(value2);
	pushOperand(value1);
	pushOperand(value4);
	pushOperand(value3);
	pushOperand(value2);
	pushOperand(value1);
}


void func_f2d(){
}

void func_f2i(){
}

void func_f2l(){
}

void func_fadd(){
	u4 value1_aux = popOperand();
	u4 value2_aux = popOperand();
	u4 result_aux;
	float value1, value2, result;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	result = value1+value2;

	memcpy(&result,&result,sizeof(u4));
}

void func_faload(){
}

void func_fastore(){
}

void func_fcmpg(){
}

void func_fcmpl(){
	u4 value1_aux = popOperand();
	u4 value2_aux = popOperand();
	u4 result;
	float value1, value2;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	if (value2>value){
		result = 1;
	}
	else if(value2==value1){
		result = 0;
	}
	else
		result = -1;
}

void func_fconst_0(){
}

void func_fconst_1(){
}

void func_fconst_2(){
}

void func_fdiv(){
	u4 value1_aux = popOperand();
	u4 value2_aux = popOperand();
	u4 result_aux;
	float value1, value2, result;


	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	if (value1 == 0){
		printf("Cannot divide by zero at fdiv");
		return;
	}

	result = value2/value1;
	memcpy(&result_aux, &result, sizeof(u4));

	pushOperand(result_aux);
}

void func_fload(){
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
	u4 value1_aux = popOperand();
	u4 value2_aux = popOperand();
	u4 result_aux;
	int q;
	float value1, value2, result;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	if (value2/value1 < 0){
		q = -1;
	}
	else
		q = 1;
	result = value2-((int)(value2/value1)*value1);
	memcpy(&result_aux, &result, sizeof(u4));

	pushOperand(result_aux);
}


void func_freturn(){
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
	u4 objectref;
}

void func_getstatic(){
}

void func_goto(){
}

void func_goto_w(){
}


void func_i2b(){
	u1 value;

	value = (char)popOperand();
	pushOperand((u4) value);

}

void func_i2c(){
}

void func_i2d(){
}

void func_i2f(){
}

void func_i2l(){
	u4 value;
	u4 aux;
	long result;

	value = (int)popOperand();
	result = value;
	aux = result >> 32;
	pushOperand(aux);
	aux = result & 0x0000FFFF;
	pushOperand(aux);

}

void func_i2s(){
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
	u4 value1 = popOperand();
	u4 value2 = popOperand();
	u4 result;

	result = value1 & value2;
	pushOperand(result);
}

void func_iastore(){
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
}

void func_if_acmpeq(){
}

void func_if_acmpne(){
}

void func_if_icmpeq(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	if (value1 == value2){
		frame_stack->frame->pc++;
		bytebranch1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		bytebranch2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (bytebranch << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}

void func_if_icmpne(){
}

void func_if_icmplt(){
}

void func_if_icmpge(){
}

void func_if_icmpgt(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	if (value1 < value2){
		frame_stack->frame->pc++;
		bytebranch1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		bytebranch2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (bytebranch << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}


void func_if_icmple(){
}

void func_ifeq(){
}

void func_ifne(){
}

void func_iflt(){
	u4 value1 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	if (value1 > 0){
		frame_stack->frame->pc++;
		bytebranch1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		bytebranch2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (bytebranch << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}


void func_ifge(){
}

void func_ifgt(){
}

void func_ifle(){
}

void func_ifnonnull(){
	u4 value1 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	if (value1 != NULL){
		frame_stack->frame->pc++;
		bytebranch1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		bytebranch2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (bytebranch << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}

void func_ifnull(){
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
	int value1 = (int)popOperand();
	int value2 = (int)popOperand();
	int result;

	result = value1*value2;
	pushOperand(result);
}

void func_ineg(){
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
	int value1 = (int)popOperand();
	int value2 = (int)popOperand();
	int result;

	if (value1 == 0){
		printf("Cannot divide by zero at irem");
		return;
	}

	result = value2-((value2/value1)*value1);
	pushOperand(result);
}

void func_ireturn(){
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
}

void func_iushr(){
}

void func_ixor(){
}


void func_jsr(){
	u2 branchbyte1, branchbyte2;
	short offset;

	frame_stack->frame->pc++;
	bytebranch1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
    frame_stack->frame->pc++;
	bytebranch2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	offset = (bytebranch << 8)|branchbyte2;
	frame_stack->frame->pc++;
	pushOperand(frame_stack->frame->pc);

	frame_stack->frame->pc += offset - 1;
}

void func_jsr_w(){
}


void func_l2d(){
}

void func_l2f(){
}

void func_l2i(){
	u4 value = popOperand();

	popOperand();
	pushOperand(value);
}

void func_ladd(){
}

void func_laload(){
}

void func_land(){
}

void func_lastore(){
	arrays_t *arrayref;
	u8 *array, value = 0;
	u4 value_hi = popOperand();
	u4 value_low = popOperand();
	int index = popOperand();

	arrayref = (array_t *)popOperand();

	value |= value_hi;
	value = value << 32;
	value |= value_low;
	if ((index >= arrayref->size) || (index < 0)){
		printf("\nNullPointerException at lasotre\n");
		return;
	}
	array = (u8 *)arrayref->reference;

	array[index]=value;

}

void func_lcmp(){
}

void func_lconst_0(){
}

void func_lconst_1(){
}

void func_ldc(){
	u1 index;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (frame_stack->frame->cp->tag == CONSTANT_Integer){
		pushOperand(frame_stack->frame->cp[index].constant_union.c_integer.bytes);
	}
	else if ((frame_stack->frame->cp->tag == CONSTANT_Float)){
		pushOperand(frame_stack->frame->cp[index].constant_union.c_float.bytes);
	}
	else if ((frame_stack->frame->cp->tag == CONSTANT_String)){
		pushOperand(getConstantPoolElementByIndex(frame_stack->frame->cp[index].constant_union.c_string.string_index));
	}
	else{
		printf("\n\ldc Error\n");
		return;
	}
}

void func_ldc_w(){
}

void func_ldc2_w(){
}

void func_ldiv(){
}

void func_lload(){
	short index, index2;
	u4 value_hi, value_low;

	frame_stack->frame->pc++;

	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (wide){
		frame_stack->frame->pc++;
		index2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		index = ((index << 8)|index2);
		wide = 0;
	}

	value_hi = frame_stack->frame->local_variables[index];
	value_low = frame_stack->frame->local_variables[index+1];

	pushOperand(value_low);
	pushOperand(value_hi);

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
}

void func_lneg(){
}

void func_lookupswitch(){
}

void func_lor(){
	u4 valor1_hi = popOperand();
	u4 valor1_low = popOperand();
	u4 valor2_hi = popOperand();
	u4 valor2_low = popOperand();
	u4 result_aux;
	u8 valor1, valor2, result;

	valor1 |= valor1_hi;
	valor1 = valor1 << 32;
	valor1 |= valor1_low;

	valor2 |= valor2_hi;
	valor2 = valor2 << 32;
	valor2 |= valor2_low;

	result = valor1 | valor2;

	result_aux = result & 0xFFFFFFFF;
	pushOperand(result_aux);
    result_aux = result >> 32;
    pushOperand(result_aux);
}

void func_lrem(){
}

void func_lreturn(){
}

void func_lshl(){
}

void func_lshr(){
	u4 value2, aux_value1, aux_value2;
	u8 value1, result;

	value2 = popOperand();
	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = (aux_value1 << 32) | aux_value2;

	result = value1 >> value2;

	aux_value1 = (result >> 32);
	aux_value2 = result & 0xFFFFFFFF;

	pushOperand(aux_value2);
	pushOperand(aux_value1);


}

void func_lstore(){
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
	u4 value2, aux_value1, aux_value2;
	u8 value1, result;

	value2 = popOperand();
	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = (aux_value1 << 32) | aux_value2;

	result = value1 ^ value2;

	aux_value1 = (result >> 32);
	aux_value2 = result & 0xFFFFFFFF;

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}


void func_monitorenter(){
}

void func_monitorexit(){
}

void func_multianewarray(){
}


void func_new(){
}

void func_newarray(){
}

void func_nop(){
}


void func_pop(){
}

void func_pop2(){
	popOperand();
	popOperand();
}

void func_putfield(){
}

void func_putstatic(){
}


void func_ret(){

}

void func_return(){
	popFrame();
}


void func_saload(){
}

void func_sastore(){
}

void func_sipush(){
}

void func_swap(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();

	pushOperand(value2);
	pushOperand(value1);
}


void func_tableswitch(){
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

