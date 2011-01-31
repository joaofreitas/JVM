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
	u2 count = 0;

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

double getDouble(u4 low_bytes, u4 high_bytes) {
	double d_value;
	u8 aux;

	aux = high_bytes;
	aux = (aux << 32) | low_bytes;
	memcpy(&d_value, &aux, sizeof(double));

	return d_value;
}

u4 getDoubleHighBytes(double value) {
	u4 high_bytes;
	u8 aux;

	memcpy(&aux, &value, sizeof(double));
	high_bytes = (aux >> 32) & 0xffffffff;
	return high_bytes;
}

u4 getDoubleLowBytes(double value) {
	u4 low_bytes;
	u8 aux;

	memcpy(&aux, &value, sizeof(double));
	low_bytes = aux & 0xffffffff;
	return low_bytes;

}

u8 getLong(u4 low_bytes, u4 high_bytes) {
	u8 l_value;

	l_value = high_bytes;
	l_value = (l_value << 32) | low_bytes;

	return l_value;
}

u4 getLongHighBytes(u8 value) {
	u4 high_bytes;

	high_bytes = (value >> 32) & 0xffffffff;
	return high_bytes;
}

u4 getLongLowBytes(u8 value) {
	u4 low_bytes;

	low_bytes = value & 0xffffffff;
	return low_bytes;
}


void func_aaload(){
	int index = popOperand();
	arrays_t *array;
	void *arrayref;
	u4 stackValue;

	array = (arrays_t*)popOperand();
	arrayref = array->reference;

	if (arrayref == NULL){
		printf("\nNull Pointer Exception at aaload.\n");
		return;
	}
	if (index >= array->size)
	{
		printf("\n\nArray Index Out Of Bounds Exception (aaload).\n");
		return;
	}

	memcpy(&stackValue, (arrayref+index*sizeof(u4)), sizeof(u4));
	pushOperand(stackValue);
}

void func_aastore(){
	u4 index, value;
    arrays_t  *array;
    void *ref;

	value = popOperand();
	index = popOperand();
	array = (arrays_t*)popOperand();

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= array->size)
	{
	    printf("\n\nArray Index Out Of Bounds Exception (op_aastore)\n");
		return;
	}
	ref = array->reference;
	memcpy((ref+index*sizeof(u4)), &value, sizeof(u4));
}

void func_aconst_null(){
	u4 *null_pointer;

	null_pointer = NULL;
	pushOperand((u4) null_pointer);
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
	unsigned char indexbyte1;
	unsigned char indexbyte2;
    arrays_t *array;
	u2 index;
	u4 count, stackValue;
	cp_info type;

	frame_stack->frame->pc++;
	indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

    type = getConstantPoolElementByIndexFromCurrentFrame(index);

	array = calloc(1, sizeof(arrays_t));
	count = (u4)popOperand();
	array->size = count;
	array->reference = calloc(count, sizeof(void *));

	memcpy(&stackValue, &array, sizeof(u4));
	pushOperand(stackValue);
}

void func_areturn(){
	frame_t *frame;
	u4 *object_reference;

	object_reference = (u4 *)popOperand();
	frame = popFrame();
	pushOperand((u4) object_reference);
}

void func_arraylength(){
	arrays_t *array;
	u4 length;

	array = (arrays_t*)popOperand();
	length = array->size;

	pushOperand(length);
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
	frame_stack->frame->local_variables[0] = popOperand();
}

void func_astore_1(){
	frame_stack->frame->local_variables[1] = popOperand();
}

void func_astore_2(){
	frame_stack->frame->local_variables[2] = popOperand();
}

void func_astore_3(){
	frame_stack->frame->local_variables[3] = popOperand();
}

void func_athrow(){
	popOperand();
	pushOperand(0);
}


void func_baload(){
	arrays_t *array_reference;
	u4 index;
	u1 value, *reference;

	index = popOperand();
	array_reference = (arrays_t *) popOperand();
	if (array_reference == NULL) {
		printf("\n\nNull Pointer Exception (op_baload)\n");
		return;
	}
	if (index >= array_reference->size)
	{
		printf("\n\nArray Index Out Of Bounds Exception (op_baload)\n");
		return;
	}
	reference = (u1 *) array_reference->reference;
	value = reference[index];
	pushOperand((u4) value);
}

void func_bastore(){
	u4 value, index;
	arrays_t *array_reference;
	u1 *ref;

	value = popOperand();
	index = popOperand();
	array_reference = (arrays_t *)popOperand();
	if (array_reference == NULL) {
		printf("\n\nNull Pointer Exception (op_bastore)\n");
		return;
	}
	if (index >= array_reference->size)
	{
		printf("\n\nArray Index Out Of Bounds Exception (op_bastore)\n");
		return;
	}
	ref = (u1*)array_reference->reference;
	ref[index] = value;
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
	arrays_t *arrey_ref;
	u4 index;
    u1 *array;

	index = popOperand();
	arrey_ref = (arrays_t *)popOperand();

	if (arrey_ref == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= arrey_ref->size)
	{
	    printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}

	array = (u1 *)arrey_ref->reference;

	pushOperand(array[index]);
}

void func_castore(){
	u4 value, index;
	arrays_t *array_reference;
	char *ref;

	value = popOperand();
	index = popOperand();
	array_reference = (arrays_t *)popOperand();
	ref = (char*)array_reference->reference;
	ref[index] = value;

}

void func_checkcast(){
/*	u4 objectref;
	u1 indexbyte1, indexbyte2;
	u2 index;
	cp_info resolved_class_type;

	objectref = popOperand();
	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = ((indexbyte1 << 8) | indexbyte2);

	resolved_class_type = getConstantPoolElementByIndexFromCurrentFrame(index);*/
	/*TODO Pode vir a ser*/

}


void func_d2f(){
	double d_value;
	float f_value;
	u4 value1, value2;

	value1 = popOperand();
	value2 = popOperand();

	d_value = getDouble(value2, value1);
	f_value = (float)d_value;

	memcpy(&value1, &f_value, sizeof(u4));
	pushOperand(value1);
}

void func_d2i(){
	long long aux1;
	double aux2=0;
	u4 aux3, aux4;

	aux1 = (long long)popOperand();
	aux4 = popOperand();
	aux1 = aux1 << 32;
	aux1 |= aux4;
	memcpy(&aux2, &aux1, sizeof(double));


	aux3 = (u4)(aux2);
	pushOperand(aux3);
}

void func_d2l(){
	u4 aux_value1, aux_value2;
	double double_value;
	long long long_value;

	aux_value1 = popOperand();
	aux_value2 = popOperand();

	double_value = getDouble(aux_value2, aux_value1);
	long_value = (long long)double_value;

	pushOperand(getLongLowBytes(long_value));
	pushOperand(getLongHighBytes(long_value));
}

void func_dadd(){
	u4 op1_hi, op1_low;
	u4 op2_hi, op2_low;
	u4 result_low, result_high;
	double op1, op2, result;

	op2_hi = popOperand();
	op2_low = popOperand();
	op1_hi = popOperand();
	op1_low = popOperand();

	op1 = getDouble(op1_low, op1_hi);
	op2 = getDouble(op2_low, op2_hi);

	result = op1+op2;
	result_low = getDoubleLowBytes(result);
	result_high = getDoubleHighBytes(result);

	pushOperand(result_low);
	pushOperand(result_high);
}

void func_daload(){
	u4 index;
	arrays_t *arrayref;
	double *ref;
	u4 low_bytes, high_bytes;

	index = popOperand();
	arrayref = (arrays_t *) popOperand();
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
	u4 index, hi, low;
    arrays_t  *array;
    double *c_array;
    double d;

	hi = popOperand();
	low = popOperand();
	index = popOperand();
	array = (arrays_t *)popOperand();

	d = getDouble(low, hi);

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= array->size || index < 0)
	{
	    printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}

	c_array = (double *)array->reference;
	c_array[index] = d;
}

void func_dcmpg(){
	double double_value1, double_value2;
	u4 value1, value2;

	value1 = popOperand();
	value2 = popOperand();
	double_value2 = getDouble(value2, value1);

	value1 = popOperand();
	value2 = popOperand();
	double_value1 = getDouble(value2, value1);

	if (double_value1 > double_value2) {
		popOperand(1);
	} else {
		if (double_value1 < double_value2) {
			popOperand(-1);
		} else {
			popOperand(0);
		}
	}
}

void func_dcmpl(){
	u4 op2_hi = popOperand();
	u4 op2_low = popOperand();
	u4 op1_hi = popOperand();
	u4 op1_low = popOperand();
	int result;
	double op1, op2;

	op1 = getDouble(op1_low, op1_hi);
	op2 = getDouble(op2_low, op2_hi);

	if (op1 < op2){
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
	u4 low_bytes = 0;
	u4 high_bytes = 0;
	popOperand(low_bytes);
	popOperand(high_bytes);
}

void func_dconst_1(){
	u4 a1,a2;

	a1 = 0x3FF00000;
	a2 = 0x00000000;
	pushOperand(a2);
	pushOperand(a1);
}

void func_ddiv(){
	double double_value1, double_value2, result;
	u4 value1, value2;
	value1 = popOperand();
	value2 = popOperand();
	double_value2 = getDouble(value2, value1);

	value1 = popOperand();
	value2 = popOperand();
	double_value1 = getDouble(value2, value1);

	result = double_value1/double_value2;

	value1 = getDoubleHighBytes(result);
	value2 = getDoubleLowBytes(result);;
	pushOperand(value2);	/*Gravo primeiro o low bytes*/
	pushOperand(value1);	/*Depois o high bytes*/
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
	u4 low_bytes, high_bytes;


	high_bytes = frame_stack->frame->local_variables[0];
	low_bytes = frame_stack->frame->local_variables[1];

	pushOperand(low_bytes);
	pushOperand(high_bytes);
}

void func_dload_1(){
	u4 low_bytes, high_bytes;

	high_bytes = frame_stack->frame->local_variables[1];
	low_bytes = frame_stack->frame->local_variables[2];

	pushOperand(low_bytes);
	pushOperand(high_bytes);
}

void func_dload_2(){
	u4 value1, value2;

	value1 = frame_stack->frame->local_variables[2];
	value2 = frame_stack->frame->local_variables[3];
	pushOperand(value2); /*Gravo primeiro o low bytes*/
	pushOperand(value1); /*Depois o high bytes*/
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

	high_bytes2 = popOperand();
	low_bytes2 = popOperand();
	high_bytes1 = popOperand();
	low_bytes1 = popOperand();

	value1 = getDouble(low_bytes1, high_bytes1);
	value2 = getDouble(low_bytes2, high_bytes2);

	result = value1*value2;
	result_high_bytes = getDoubleHighBytes(result);
	result_low_bytes = getDoubleLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_dneg(){
	u4 hi,low;
	u4 a1 = 0x80000000;

	hi = popOperand();
	low = popOperand();

	hi ^= a1;

	pushOperand(low);
	pushOperand(hi);
}

void func_drem(){
	double double_value1, double_value2, result;
	u4 value1, value2;
	value1 = popOperand();
	value2 = popOperand();
	double_value1 = getDouble(value2, value1);

	value1 = popOperand();
	value2 = popOperand();
	double_value2 = getDouble(value2, value1);

	result = (double_value2 - ( (int) (double_value2/double_value1) ) * (double_value1));
	value1 = getDoubleHighBytes(result);
	value2 = getDoubleLowBytes(result);
	pushOperand(value2); /*Gravo primeiro o low bytes*/
	pushOperand(value1); /*Depois o high bytes*/
}

void func_dreturn(){
	u4 value_hi = popOperand();
	u4 value_low = popOperand();

	popFrame();

	pushOperand(value_low);
	pushOperand(value_hi);
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
	frame_stack->frame->local_variables[0] = popOperand();/*Pego primeiro o high bytes*/
	frame_stack->frame->local_variables[1] = popOperand();/*Depois low bytes*/
}

void func_dstore_1(){
	frame_stack->frame->local_variables[1] = popOperand();/*Pego primeiro o high bytes*/
	frame_stack->frame->local_variables[2] = popOperand();/*Depois low bytes*/
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
	u4 hi,low;
	double d1,d2;

	hi  = popOperand();
	low = popOperand();
	d1  = getDouble(low,hi);

	hi  = popOperand();
	low = popOperand();
	d2  = getDouble(low,hi);

    d1 = d2 - d1;

    pushOperand(getDoubleLowBytes(d1));
    pushOperand(getDoubleHighBytes(d1));
}

void func_dup(){
	u4 value;

	value = popOperand();
	pushOperand(value);
	pushOperand(value);
}

void func_dup_x1(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();

	pushOperand(value1);
	pushOperand(value2);
	pushOperand(value1);
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
	u4 a1 , a2;

	a1 = popOperand();
	a2 = popOperand();

	pushOperand(a2);
	pushOperand(a1);
	pushOperand(a2);
	pushOperand(a1);
}

void func_dup2_x1(){
	u4 value1, value2, value3;
	value1 = popOperand();
	value2 = popOperand();
	value3 = popOperand();
	pushOperand(value2);
	pushOperand(value1);
	pushOperand(value3);
	pushOperand(value2);
	pushOperand(value1);
}

void func_dup2_x2(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();
	u4 value3 = popOperand();
	u4 value4 = popOperand();

	pushOperand(value2);
	pushOperand(value1);
	pushOperand(value4);
	pushOperand(value3);
	pushOperand(value2);
	pushOperand(value1);
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
    float f;
    u4 a1;

    a1 = popOperand();
	memcpy(&f, &a1, sizeof(float));
	a1 = (int)f;
	pushOperand(a1);
}

void func_f2l(){
	float float_value;
	u4 aux_value, aux_value2;
	u8 result;

	aux_value = popOperand();

	memcpy(&float_value, &aux_value, sizeof(float));

	result = (u8) float_value;

	aux_value = getLongHighBytes(result);
	aux_value2 = getLongLowBytes(result);
	pushOperand(aux_value2);
	pushOperand(aux_value);
}

void func_fadd(){
	u4 value2_aux = popOperand();
	u4 value1_aux = popOperand();
	float value1, value2, result;
	u4 stackValue;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	result = value1+value2;

	memcpy(&stackValue,&result,sizeof(u4));
	pushOperand(stackValue);
}

void func_faload(){
	u4 index;
	arrays_t *arrayref;
	float *ref;
	u4 stackValue;

	index = popOperand();
	arrayref = (arrays_t *)popOperand();
	if (arrayref == NULL){
		printf("\nNullPointerException at faload.\n");
		return;
	}
	if (index >= arrayref->size || index < 0){
		printf("\nArrayIndexOutOfBoundsException at faload.\n");
		return;
	}

	ref = (float *) arrayref->reference;
	memcpy(&stackValue, &ref[index], sizeof(u4));

	pushOperand(stackValue);
}

void func_fastore(){
	u4 index, value;
    arrays_t  *array;
    float *c_array;
    float f;

	value = popOperand();
	index = popOperand();
	array = (arrays_t *)popOperand();
    memcpy(&f, &value, sizeof(float));

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (func_fastore)\n");
		return;
	}
	if (index >= array->size || index < 0)
	{
	    printf("\n\nNull Pointer Exception (func_fastore)\n");
		return;
	}

	c_array = (float *)array->reference;
	c_array[index] = f;
}

void func_fcmpg(){
	float float_value1, float_value2;
	u4 value1, value2;

	value2 = popOperand();
	value1 = popOperand();

	memcpy(&float_value1, &value1, sizeof(float));
	memcpy(&float_value2, &value2, sizeof(float));

	if (float_value1 > float_value2) {
		pushOperand(1);
	} else {
		if (float_value1 < float_value2) {
			pushOperand(-1);
		} else {
			pushOperand(0);
		}
	}

}

void func_fcmpl(){
	u4 value2_aux = popOperand();
	u4 value1_aux = popOperand();
	float value1, value2;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	if (value1 < value2) {
		pushOperand(1);
	}
	else if(value1 == value2){
		pushOperand(0);
	}
	else {
		pushOperand(-1);
	}
}

void func_fconst_0(){
	float value = 0;
	u4 float_number;

	memcpy(&float_number, &value, sizeof(float));
	pushOperand(float_number);
}

void func_fconst_1(){
	float value = 1.0;
	u4 float_number;

	memcpy(&float_number, &value, sizeof(float));
	pushOperand(float_number);
}

void func_fconst_2(){
	float value = 2.0;
	u4 float_number;

	memcpy(&float_number, &value, sizeof(float));
	pushOperand(float_number);

}

void func_fdiv(){
	u4 value2_aux = popOperand();
	u4 value1_aux = popOperand();
	u4 result_aux;
	float value1, value2, result;


	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	if (value2 == 0){
		printf("Cannot divide by zero at fdiv");
		return;
	}

	result = value1/value2;
	memcpy(&result_aux, &result, sizeof(u4));

	pushOperand(result_aux);
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
	pushOperand(frame_stack->frame->local_variables[0]);
}

void func_fload_1(){
	pushOperand(frame_stack->frame->local_variables[1]);
}

void func_fload_2(){
	pushOperand(frame_stack->frame->local_variables[2]);
}

void func_fload_3(){
	pushOperand(frame_stack->frame->local_variables[3]);
}

void func_fmul(){
	u4 a1, a2;
	float f1, f2;

	a1 = popOperand();
	a2 = popOperand();
	memcpy(&f1, &a1, sizeof(float));
	memcpy(&f2, &a2, sizeof(float));

	f1 *= f2;
	memcpy(&a1, &f1, sizeof(u4));

	pushOperand(a1);
}

void func_fneg(){
	u4 value;
	float float_number;

	value = popOperand();

	memcpy(&float_number, &value, sizeof(float));
	float_number = -float_number;
	memcpy(&value, &float_number, sizeof(float));
	pushOperand(value);
}

void func_frem(){
	u4 value1_aux = popOperand();
	u4 value2_aux = popOperand();
	u4 result_aux;
	float value1, value2, result;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	result = value2-(value1*(value2/value1));
	memcpy(&result_aux, &result, sizeof(u4));

	pushOperand(result_aux);
}

void func_freturn(){
	u4 value;
	value = popOperand();
	popFrame();
	pushOperand(value);
}

void func_fstore(){
	u4 a1;
	u2 index;

    frame_stack->frame->pc++;
    index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	a1 = popOperand();
	frame_stack->frame->local_variables[index] = a1;
}

void func_fstore_0(){

	frame_stack->frame->local_variables[0] = popOperand(); /* Gravo o endereço para o float */
}

void func_fstore_1(){
	frame_stack->frame->local_variables[1] = popOperand(); /* Gravo o endereço para o float */
}

void func_fstore_2(){
	frame_stack->frame->local_variables[2] = popOperand(); /* Gravo o endereço para o float */
}

void func_fstore_3(){
	frame_stack->frame->local_variables[3] = popOperand(); /* Gravo o endereço para o float */
}

void func_fsub(){
	u4 value1, value2;
	float float_value1, float_value2, result;

	value2 = popOperand();
	value1 = popOperand();

	memcpy(&float_value1, &value1, sizeof(float));
	memcpy(&float_value2, &value2, sizeof(float));

	result = float_value1 - float_value2;

	memcpy(&value1, &result, sizeof(u4));
	pushOperand(value1);	/* Gravo o endereço para o float */
}


void func_getfield(){
	u1 *field_descriptor, *field_name, *class_name;
	u4 indexbyte1, indexbyte2;
	u4 low_bytes, high_bytes;
	u4 index;
	u4 field_index;
	u4 class_index;
	u8 value;
	instance_structure *objectref;
	instance_variables *resolved_instance_variable;
	class *field_class;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;

	class_index = getConstantPoolElementByIndexFromCurrentFrame(index).constant_union.c_fieldref.class_index;
	class_name = getConstantPoolElementByIndexFromCurrentFrame(class_index).constant_union.c_utf8.bytes;
	field_class = getClass((char *)class_name);
	field_descriptor = getFieldDescriptor(field_class, index);
	field_index = getFieldIndex(field_class, index);
	field_name = getFieldName(field_class->class_file->constant_pool, field_index);

	objectref = (instance_structure *) popOperand();
	resolved_instance_variable = getResolvedInstanceVariables(objectref, field_descriptor, field_name);
	value = resolved_instance_variable->value;

	if((field_descriptor[0] == 'J') || (field_descriptor[0] == 'D')) {
		low_bytes = getLongLowBytes(value);
		high_bytes = getLongHighBytes(value);
		pushOperand(low_bytes);
		pushOperand(high_bytes);
	} else {
		pushOperand(getLongLowBytes(value));
	}

}

void func_getstatic(){
	u4 indexbyte1, indexbyte2;
	u4 index;
	class *cl;
	cp_info field, class_name_info;
	field_info *fieldref;
	char *class_name;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;

	field = getConstantPoolElementByIndexFromCurrentFrame(index);
	class_name_info = getConstanPoolElement(field.constant_union.c_fieldref.class_index);
	class_name = (char *) getConstanPoolElement(class_name_info.constant_union.c_class.name_index).constant_union.c_utf8.bytes;

	if(strcmp((char *)class_name, "java/lang/System") == 0) {
		return;
	}

	cl = getSymbolicReferenceClass(class_name);

	fieldref = getResolvedFieldReference(cl, field);

	if(fieldref == NULL) {
		return;
	}

	/*value = getConstantPoolElementByIndexFromCurrentFrame(fieldref->attributes.attribute_union.constant_value.constant_value_index);
	pushOperand();*/
}

void func_goto(){
	unsigned char indexbyte1;
	unsigned char indexbyte2;
	u2 index;

	frame_stack->frame->pc++;
    indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

	frame_stack->frame->pc += index;
	frame_stack->frame->pc--;
}

void func_goto_w(){
	u2 branchbyte1, branchbyte2, branchbyte3, branchbyte4;
	u4 offset;

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte3 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte4 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	offset = (branchbyte1 << 24) | (branchbyte2 << 16) | (branchbyte3 << 8) | branchbyte4;

	frame_stack->frame->pc += (int) offset - 1;
}


void func_i2b(){
	u1 value;

	value = (u1)popOperand();
	pushOperand((u4) value);

}

void func_i2c(){
	u4 value;

	value = (u1)popOperand();
	value &= 0x000000ff;
	pushOperand(value);
}

void func_i2d(){
	u4 hi,low;
	int uint;
	double d;

	uint = popOperand();
    d = (double)uint;

    hi = getDoubleHighBytes(d);
    low = getDoubleLowBytes(d);

    pushOperand(low);
    pushOperand(hi);
}

void func_i2f(){
	int value;
	float result;

	value = (int) popOperand();
	result = (float) value;
	memcpy(&value, &result, sizeof(float));
	pushOperand(value);
}

void func_i2l(){
	u4 value;
	u4 aux;
	long result;

	value = (int)popOperand();
	result = (long) value;
	aux = getLongLowBytes(result);
	pushOperand(aux);
	aux = getLongHighBytes(result);
	pushOperand(aux);

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
	u4 index;
	arrays_t *array_reference;
	int *ref;
	u4 stackValue;

	index = popOperand();
	array_reference = (arrays_t *)popOperand();
	if (array_reference == NULL) {
		printf("NullPointerException!\n");
		exit(0);
	}
	if (index >= array_reference->size)
	{
		printf("\n\nArray Index Out Of Bounds Exception (aaload).\n");
		return;
	}
	ref = (int*) array_reference->reference;
	memcpy(&stackValue, &ref[index], sizeof(u4));
	pushOperand(stackValue);
}

void func_iand(){
	u4 value2 = popOperand();
	u4 value1 = popOperand();
	u4 result;

	result = value1 & value2;
	pushOperand(result);
}

void func_iastore(){
	u4 index;
	arrays_t *arrayref;
	int *ref;
	u4 stackValue;

	stackValue = popOperand();
	index = popOperand();
	arrayref = (arrays_t *)popOperand();
	ref = (int *)arrayref->reference;

	if (arrayref == NULL){
		printf("\nNullPointerException at iastore.\n");
		return;
	}
	if (index >= arrayref->size || index < 0){
		printf("\n ArrayIndexOutOfBoundsException at iastore.\n");
		return;
	}
	ref[index] = stackValue;
}

void func_iconst_m1(){
	pushOperand(-1);
}

void func_iconst_0(){
	pushOperand(0);

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
		printf("\n ArithmeticException at idiv.\n");
	}

	pushOperand((u4)value1/value2);
}

void func_if_acmpeq(){
	u4 value1, value2;
	u2 branchbyte1, branchbyte2, offset;

	value2 = popOperand();
	value1 = popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 == value2) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_if_acmpne(){
	u4 value1, value2;
	u2 branchbyte1, branchbyte2, offset;

	value2 = popOperand();
	value1 = popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 != value2) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_if_icmpeq(){
	u4 value2 = popOperand();
	u4 value1 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	frame_stack->frame->pc++;
	branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 == value2){
		offset = (branchbyte1 << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}

void func_if_icmpne(){
	int value1, value2;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value2 = popOperand();
	value1 = popOperand();

	frame_stack->frame->pc++;
	brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if(value1 != value2) {
		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
		}
	}
}

void func_if_icmplt(){
	int value1, value2;
	u2 branchbyte1, branchbyte2, offset;

	value2 = (int) popOperand();
	value1 = (int) popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 < value2) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 1;
	}
}

void func_if_icmpge(){
	int value1, value2;
	u2 branchbyte1, branchbyte2, offset;

	value2 = (int) popOperand();
	value1 = (int) popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 >= value2) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 1;
	}
}

void func_if_icmpgt(){
	u4 value2 = popOperand();
	u4 value1 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	frame_stack->frame->pc++;
	branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 > value2){
		offset = (branchbyte1 << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}

void func_if_icmple(){
	int value1, value2;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value2 = popOperand();
	value1 = popOperand();

	frame_stack->frame->pc++;
	brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if(value1 <= value2) {
		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
		}
	}
}

void func_ifeq(){
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 == 0) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 1;
	}
}

void func_ifne(){
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 != 0) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 1;
	}
}

void func_iflt(){
	int value1 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	frame_stack->frame->pc++;
	branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 < 0){
		offset = (branchbyte1 << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}

void func_ifge(){
	int value;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value = popOperand();

	frame_stack->frame->pc++;
	brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if(value >= 0) {
		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
		}
	}
}

void func_ifgt(){
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 > 0) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 1;
	}
}

void func_ifle(){
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 <= 0) {
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 1;
	}
}

void func_ifnonnull(){
	u4 *value1;
	u2 branchbyte1, branchbyte2;
	short offset;

	value1 = (u4 *) popOperand();

	frame_stack->frame->pc++;
	branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (value1 != NULL){
		offset = (branchbyte1 << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}

void func_ifnull(){
	u4 *value;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value = (u4 *) popOperand();

	frame_stack->frame->pc++;
	brenchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	brenchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if(value == NULL) {
		offset = (brenchbyte1 << 8) | brenchbyte2;
		if ((frame_stack->frame->pc + offset) < frame_stack->frame->code_length) {
			frame_stack->frame->pc += offset-1;
		}
	}
}

void func_iinc(){
	u4 constant;
	int index, index2;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (wide){
		frame_stack->frame->pc++;
		index2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		index = ((index << 8)|index2);
		frame_stack->frame->pc++;
		constant = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		constant = constant << 8;
		constant |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		wide = 0;
	}
	else{
		frame_stack->frame->pc++;
		constant = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	}
	frame_stack->frame->local_variables[index] += constant;
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
	pushOperand(frame_stack->frame->local_variables[0]);
}

void func_iload_1(){
	pushOperand(frame_stack->frame->local_variables[1]);
}

void func_iload_2(){
	 pushOperand(frame_stack->frame->local_variables[2]);
}

void func_iload_3(){
	 pushOperand(frame_stack->frame->local_variables[3]);
}

void func_imul(){
	int value1 = (int)popOperand();
	int value2 = (int)popOperand();
	int result;

	result = value1*value2;
	pushOperand(result);
}

void func_ineg(){
	int value;

	value = popOperand();
	pushOperand((~value)+1);
}

void func_instanceof(){
	/*nao implementado*/
}

void func_invokeinterface(){
	/*nao implementado*/
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

void println(char *descriptor) {
	u4 value;
	u4 long_value_high, long_value_low;
	long long long_value;
	cp_info cp;
	float float_value;
	double double_value;

	if((strcmp(descriptor, "(C)V") == 0)) {
		printf("%c\n", popOperand());
	}
	else if (strcmp(descriptor, "()V") == 0) {
		printf("\n");
	}
	else if (strcmp(descriptor, "(I)V") == 0) {
		printf("%d\n", popOperand());
	}
	else if (strcmp(descriptor, "(Z)V") == 0) {
		printf(popOperand() ? "true\n" : "false\n");
	}
	else if (strcmp(descriptor, "(F)V") == 0) {
		value = popOperand();
		memcpy(&float_value, &value, sizeof(u4));
		printf("%f\n", float_value);
	}
	else if (strcmp(descriptor, "(J)V") == 0) {
		long_value_high = popOperand();
		long_value_low = popOperand();

		long_value = (long long) getLong(long_value_low, long_value_high);

		printf("%lld\n", long_value);
	}
	else if (strcmp(descriptor, "(D)V") == 0) {
		long_value_high = popOperand();
		long_value_low = popOperand();

		double_value = getDouble(long_value_low, long_value_high);

		printf("%.5f\n", double_value);
	}
	else if (strcmp((char *) descriptor, "(Ljava/lang/String;)V") == 0) {
		cp = getConstantPoolElementByIndexFromCurrentFrame(popOperand());
		printf("%s\n", (char *)cp.constant_union.c_utf8.bytes);
	}

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

	method_ref_info = getConstantPoolElementByIndexFromCurrentFrame(index);

	/*Pegando o nome da classe*/
	class_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.class_index);
	class_name = (char *)getConstanPoolElement(class_info.constant_union.c_class.name_index).constant_union.c_utf8.bytes;

	/*Pegando o nome e o descritor do método*/
	method_name_type_ref_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.name_and_type_index);
	method_name = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.name_index).constant_union.c_utf8.bytes;
	method_descriptor = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.descriptor_index).constant_union.c_utf8.bytes;

	if((strcmp(class_name, "java/io/PrintStream") == 0) && (strcmp(method_name, "println") == 0)) {
		println((char *)method_descriptor);
		return;
	}

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
	u4 value1, value2;

	value1 = popOperand();
	value2 = popOperand();
	pushOperand((u4) value1|value2);
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
	u4 value;

	value = popOperand();
	popFrame();
	pushOperand(value);
}

void func_ishl(){
	int value1, value2, result;

		value2 = (int) popOperand();
		value1 = (int) popOperand();

		value2 = value2 & 0x1f;
		result = value1 << value2;
		pushOperand(result);
}

void func_ishr(){
	int value1, value2, result;

	value2 = (int) popOperand();
	value1 = (int) popOperand();

	value2 = value2 & 0x1f;
	result = value1 >> value2;
	pushOperand(result);
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
	u4 a1, a2;

	a2 = popOperand();
	a1 = popOperand();
	a2 = a2 & 0x1f;
	pushOperand(a1 >> a2);
}

void func_ixor(){
	int value1, value2, result;

	value1 = (int) popOperand();
	value2 = (int) popOperand();

	result = value1 ^ value2;
	pushOperand((u4 )result);
}


void func_jsr(){
	u2 branchbyte1, branchbyte2;
	short offset;

	frame_stack->frame->pc++;
	branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
    frame_stack->frame->pc++;
    branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	offset = (branchbyte1 << 8)|branchbyte2;
	frame_stack->frame->pc++;
	pushOperand(frame_stack->frame->pc);

	frame_stack->frame->pc += offset - 1;
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
	u4 long_low_bytes;
	u4 long_high_bytes;
	u8 long_value;
	double double_value;

	long_high_bytes = popOperand();
	long_low_bytes = popOperand();
	long_value = getLong(long_low_bytes, long_high_bytes);
	double_value = (double)long_value;

	pushOperand(getDoubleLowBytes(double_value));
	pushOperand(getDoubleHighBytes(double_value));
}

void func_l2f(){
	u4 aux_value1, aux_value2;
	u8 long_value;
	float result;

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	long_value = getLong(aux_value2, aux_value1);

	result = (float) long_value;

	memcpy(&aux_value1, &result, sizeof(float));

	pushOperand(aux_value1);
}

void func_l2i(){
	u4 high_bytes, low_bytes;
	int int_value;
	u8 long_value;

	high_bytes = popOperand();
	low_bytes = popOperand();

	long_value = getLong(low_bytes, high_bytes);
	int_value = (int) long_value;

	pushOperand(int_value);
}

void func_ladd(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	high_bytes2 = popOperand();
	low_bytes2 = popOperand();

	high_bytes1 = popOperand();
	low_bytes1 = popOperand();

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);
	result = value1+value2;
	result_high_bytes = getLongHighBytes(result);
	result_low_bytes = getLongLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_laload(){
	arrays_t *arrey_ref;
	u4 index;
	u8 *array;
	u4 hi, low;

	index = popOperand();
	arrey_ref = (arrays_t *)popOperand();

	if (arrey_ref == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= arrey_ref->size)
	{
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}

	array = (u8 *)arrey_ref->reference;

	hi = (array[index]>>32);
	low = (array[index] & 0xffffffff);

	pushOperand(low);
	pushOperand(hi);
}

void func_land(){
	u4 aux_value1, aux_value2;
	u8 result, value1, value2;

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value2 = getLong(aux_value2, aux_value1);

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 & value2;
	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongLowBytes(result);

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}

void func_lastore(){
	arrays_t *arrayref;
	u8 *array, value = 0;
	u4 value_hi = popOperand();
	u4 value_low = popOperand();
	int index = popOperand();

	arrayref = (arrays_t *)popOperand();

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
	u8 value1, value2;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;

	high_bytes2 = popOperand();
	low_bytes2 = popOperand();
	high_bytes1 = popOperand();
	low_bytes1 = popOperand();
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
	pushOperand(0);
	pushOperand(0);
}

void func_lconst_1(){
	pushOperand(1);
	pushOperand(0);
}

void func_ldc(){
	u1 index;
	cp_info cp;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	cp = getConstantPoolElementByIndexFromCurrentFrame(index);

	if (cp.tag == 3){
		pushOperand(cp.constant_union.c_integer.bytes);
	}
	else if ((cp.tag == 4)){
		pushOperand(cp.constant_union.c_float.bytes);
	}
	else if ((cp.tag == 8)){
		/*cp_string = getConstantPoolElementByIndexFromCurrentFrame();*/
		pushOperand(cp.constant_union.c_string.string_index);
	}
	else{
		printf("\n\t Error\n");
		return;
	}
}

void func_ldc_w(){
	u4 indexbyte1, indexbyte2;
	u4 index;
	cp_info info;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;

	info = getConstantPoolElementByIndexFromCurrentFrame(index);
	switch(info.tag)
	{
		case 3:/*CONSTANT_Integer:*/
		{
			pushOperand(info.constant_union.c_integer.bytes);
		}
		break;
		case 4:/*CONSTANT_Float:*/
		{
			pushOperand(info.constant_union.c_float.bytes);
		}
		break;
		default:
			printf("\n ldc_w ERROR\n");
		break;
	}

}

void func_ldc2_w(){
	u4 indexbyte1, indexbyte2;
	u4 index;
	cp_info info;
	u4 low_bytes, high_bytes;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;

	info = getConstantPoolElementByIndexFromCurrentFrame(index);
	switch(info.tag)
	{
		case 5:/*CONSTANT_Long:*/
		{
			low_bytes = info.constant_union.c_long.low_bytes;
			high_bytes = info.constant_union.c_long.high_bytes;
			pushOperand(low_bytes);
			pushOperand(high_bytes);
		}
		break;
		case 6:/*CONSTANT_Double:*/
		{
			low_bytes = info.constant_union.c_double.low_bytes;
			high_bytes = info.constant_union.c_double.high_bytes;
			pushOperand(low_bytes);
			pushOperand(high_bytes);
		}
		break;
		default:
			printf("\n ldc_w ERROR\n");
		break;
	}
}

void func_ldiv(){
	u4 aux_value1, aux_value2;
	u8 result, value1, value2;

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value2 = getLong(aux_value2, aux_value1);

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 / value2;
	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongLowBytes(result);

	pushOperand(aux_value2);
	pushOperand(aux_value1);
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
	u4 aux_value1, aux_value2;

	aux_value1 = frame_stack->frame->local_variables[0];
	aux_value2 = frame_stack->frame->local_variables[1];

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}

void func_lload_1(){
	u4 aux_value1, aux_value2;

	aux_value1 = frame_stack->frame->local_variables[1];
	aux_value2 = frame_stack->frame->local_variables[2];

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}

void func_lload_2(){
	u4 aux_value1, aux_value2;

	aux_value1 = frame_stack->frame->local_variables[2];
	aux_value2 = frame_stack->frame->local_variables[3];

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}

void func_lload_3(){
	u4 aux_value1, aux_value2;

	aux_value1 = frame_stack->frame->local_variables[3];
	aux_value2 = frame_stack->frame->local_variables[4];

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}

void func_lmul(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	high_bytes2 = popOperand();
	low_bytes2 = popOperand();

	high_bytes1 = popOperand();
	low_bytes1 = popOperand();

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);
	result = value1*value2;
	result_high_bytes = getLongHighBytes(result);
	result_low_bytes = getLongLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_lneg(){
	u4 a2, a1;
	u8 long_value;

	a1 = popOperand();
	a2 = popOperand();
	long_value = getLong(a2, a1);
	long_value = -long_value;
	a2 = getLongLowBytes(long_value);
	a1 = getLongHighBytes(long_value);
	pushOperand(a2);
	pushOperand(a1);
}

void func_lookupswitch(){
	u4 opcode, default_byte, number_pairs, i, j, key, found;
	pair_item *pair;

	opcode = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;

	while (frame_stack->frame->pc %4 !=0){
		frame_stack->frame->pc++;
	}

	default_byte = 0;
	for(i = 0; i < 3; i++) {
		default_byte |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		default_byte = default_byte << 8;
		frame_stack->frame->pc++;
	}

	default_byte |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;

	number_pairs = 0;
	for(i = 0; i < 3; i++)
	{
		number_pairs |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		number_pairs = number_pairs << 8;
		frame_stack->frame->pc++;
	}

	number_pairs |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;

	pair = malloc(sizeof(pair_item) * number_pairs);

	for(i = 0; i < number_pairs; i++) {
		pair->match = 0;

		for(j = 0; j < 3; j++) {
			pair->match |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
			pair->match = pair->match << 8;
			frame_stack->frame->pc++;
		}

		pair->match |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;

		pair->offset = 0;

		for(j = 0; j < 3; j++) {
			pair->offset |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
			pair->offset = pair->offset << 8;
			frame_stack->frame->pc++;
		}

		pair->offset |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		pair++;
	}

	key = popOperand();
	found = 0;
	i = 0;

	while ((!found) && (i < number_pairs)) {
		if (key == pair->match) {
			found = 1;
			frame_stack->frame->pc = pair->offset + opcode;
		} else {
			pair++;
			i++;
		}
	}

	if(!found) {
		frame_stack->frame->pc = default_byte + 2;
	}
}

void func_lor(){
	u4 valor2_hi = popOperand();
	u4 valor2_low = popOperand();
	u4 valor1_hi = popOperand();
	u4 valor1_low = popOperand();
	u8 valor1, valor2, result;

	valor1 = getLong(valor1_low, valor1_hi);
	valor2 = getLong(valor2_low, valor2_hi);

	result = valor1 | valor2;

	pushOperand(getLongLowBytes(result));
    pushOperand(getLongHighBytes(result));
}

void func_lrem(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	high_bytes2 = popOperand();
	low_bytes2 = popOperand();

	high_bytes1 = popOperand();
	low_bytes1 = popOperand();

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);
	if (value2 == 0) {
		printf("\nArithmeticException at idiv.\n");
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
	u4 value2, aux_value1, aux_value2;
	u8 value1, result;

	value2 = popOperand();
	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 << value2;

	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongLowBytes(result);

	pushOperand(aux_value2);
	pushOperand(aux_value1);

}

void func_lshr(){
	u4 value2, aux_value1, aux_value2;
	u8 value1, result;

	value2 = popOperand();
	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 >> value2;

	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongLowBytes(result);

	pushOperand(aux_value2);
	pushOperand(aux_value1);


}

void func_lstore(){	
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
	u4 aux_value1, aux_value2;

	aux_value1 = popOperand();
	aux_value2 = popOperand();

	frame_stack->frame->local_variables[0] = aux_value1;
	frame_stack->frame->local_variables[1] = aux_value2;
}

void func_lstore_1(){
	u4 aux_value1, aux_value2;

	aux_value1 = popOperand();
	aux_value2 = popOperand();

	frame_stack->frame->local_variables[1] = aux_value1;
	frame_stack->frame->local_variables[2] = aux_value2;

}

void func_lstore_2(){
	u4 aux_value1, aux_value2;

	aux_value1 = popOperand();
	aux_value2 = popOperand();

	frame_stack->frame->local_variables[2] = aux_value1;
	frame_stack->frame->local_variables[3] = aux_value2;
}

void func_lstore_3(){
	u4 aux_value1, aux_value2;

	aux_value1 = popOperand();
	aux_value2 = popOperand();

	frame_stack->frame->local_variables[3] = aux_value1;
	frame_stack->frame->local_variables[4] = aux_value2;
}

void func_lsub(){
	u8 value1, value2, result;
	u4 high_bytes1, low_bytes1;
	u4 high_bytes2, low_bytes2;
	u4 result_high_bytes, result_low_bytes;

	high_bytes2 = popOperand();
	low_bytes2 = popOperand();

	high_bytes1 = popOperand();
	low_bytes1 = popOperand();

	value1 = getLong(low_bytes1, high_bytes1);
	value2 = getLong(low_bytes2, high_bytes2);
	result = value1-value2;
	result_high_bytes = getLongHighBytes(result);
	result_low_bytes = getLongLowBytes(result);

	pushOperand(result_low_bytes);
	pushOperand(result_high_bytes);
}

void func_lushr(){
	u4 aux_value1, aux_value2;
	int aux_value3;
	u8 value1, result;

	aux_value3 = popOperand();
	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	if (value1 > 0) {
		result = value1 >> aux_value3;
	} else {
		result = (value1 >> aux_value3) + ((long)2 << ~aux_value3);
	}

	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongLowBytes(result);

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}

void func_lxor(){
	u4 aux_value1, aux_value2;
	u8 value1, value2, result;


	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value2 = getLong(aux_value2, aux_value1);

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 ^ value2;

	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongLowBytes(result);

	pushOperand(aux_value2);
	pushOperand(aux_value1);
}


void func_monitorenter(){
	/*Nao implementado*/
}

void func_monitorexit(){
	/*Nao implementado*/
}

void add_multiarray(u4 ** ponteiro, u4 * tamanhos, u4 dimensoes ){
	int i, j;
	if(dimensoes == 0){
		return;
	}
	i = tamanhos[dimensoes-1];
	*ponteiro = (u4 *)malloc(i*sizeof(u4 *));
	//add_array(*ponteiro, i);
	for(j = 0; j < i; j++){
		//		printf("criado array: %i %i %x\n", i, j, ponteiro);
		add_multiarray(&(ponteiro[j]), tamanhos, dimensoes-1);
	}
}

void func_multianewarray(){
	u2 index_byte1, index_byte2;
	u4 stack_v, dimension, *size;
	arrays_t *array_reference  = calloc(1,sizeof(arrays_t));
	u4 *array;
	int i;


	frame_stack->frame->pc++;
	index_byte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	index_byte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	frame_stack->frame->pc++;
	dimension = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	size = (u4 *) malloc(dimension*sizeof(u4));
	for(i = 0; i < dimension; i++){
		size[i] = popOperand();
	}
	add_multiarray(&array, size, dimension);

	array_reference->reference = (void *)(array);

	memcpy(&stack_v, &array_reference, sizeof(arrays_t));
	pushOperand(stack_v);

}


void func_new(){
	u4 indexbyte1, indexbyte2;
	u4 index;
	cp_info simbolicRef;
	class *cl;
	char *refName;
	instance_structure *obj;
	u4 stackValue;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = (indexbyte1 << 8) | indexbyte2;

	simbolicRef = getConstantPoolElementByIndexFromCurrentFrame(index);
	refName = (char *)getConstantPoolElementByIndexFromCurrentFrame(simbolicRef.constant_union.c_class.name_index).constant_union.c_utf8.bytes;
	cl = getSymbolicReferenceClass(refName);
	obj = instanceClass(cl);

	memcpy(&stackValue, &obj, sizeof(u4));
	pushOperand(stackValue);
}

void func_newarray(){
	arrays_t *array;
	u4 stackValue;
	u4 count;
	u1 type;

	frame_stack->frame->pc++;
	type = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	count = (u4)popOperand();

	array = calloc(1, sizeof(arrays_t));
	array->size = count;
	array->type = type;
	switch (type)
	{
		case 4:
			array->reference = calloc(count, sizeof(char));
			break;
		case 5:
			array->reference = calloc(count, sizeof(char));
			break;
		case 6:
			array->reference = calloc(count, sizeof(float));
			break;
		case 7:
			array->reference = calloc(count, sizeof(double));
			break;
		case 8:
			array->reference = calloc(count, sizeof(unsigned char));
			break;
		case 9:
			array->reference = calloc(count, sizeof(short int));
			break;
		case 10:
			array->reference = calloc(count, sizeof(int));
			break;
		case 11:
			array->reference = calloc(count, sizeof(long long));
			break;
	}

	memcpy(&stackValue, &array, sizeof(u4));
	pushOperand(stackValue);
}

void func_nop(){
	/*Nothing*/
}


void func_pop(){
	popOperand();
}

void func_pop2(){
	popOperand();
	popOperand();
}

void func_putfield(){
	u1 *field_descriptor, *class_name;
	u4 indexbyte1, indexbyte2;
	u4 low_bytes, high_bytes;
	u4 index;
	u4 field_index;
	u1 *field_name;
	u4 class_index;
	u8 value;
	instance_structure *objectref;
	instance_variables *resolved_instance_variable;
	class *field_class;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;

	class_index = getConstantPoolElementByIndexFromCurrentFrame(index).constant_union.c_fieldref.class_index;
	class_name = getConstantPoolElementByIndexFromCurrentFrame(class_index).constant_union.c_utf8.bytes;
	field_class = getClass((char *)class_name);
	field_descriptor = getFieldDescriptor(field_class, index);
	field_index = getFieldIndex(field_class, index);
	field_name = getFieldName(field_class->class_file->constant_pool, field_index);

	if((field_descriptor[0] == 'J') || (field_descriptor[0] == 'D')) {
		low_bytes = popOperand();
		high_bytes = popOperand();
		value = getLong(low_bytes, high_bytes);
	} else {
		value = getLong(popOperand(), 0x00000000);
	}

	objectref = (instance_structure *) popOperand();
	resolved_instance_variable = getResolvedInstanceVariables(objectref, field_descriptor, field_name);
	resolved_instance_variable->value = value;
}

void func_putstatic(){
	u1 *field_descriptor, *class_name;
	u4 indexbyte1, indexbyte2;
	u4 low_bytes, high_bytes;
	u4 index;
	u4 field_index;
	u4 class_index;
	u8 value;
	class *field_class;

	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	index = (indexbyte1 << 8) | indexbyte2;

	class_index = getConstantPoolElementByIndexFromCurrentFrame(index).constant_union.c_fieldref.class_index;
	class_name = getConstantPoolElementByIndexFromCurrentFrame(class_index).constant_union.c_utf8.bytes;
	field_class = getClass((char *)class_name);
	field_descriptor = getFieldDescriptor(field_class, index);
	field_index = getFieldIndex(field_class, index);

	if((field_descriptor[0] == 'J') || (field_descriptor[0] == 'D')) {
		low_bytes = popOperand();
		high_bytes = popOperand();
		value = getLong(low_bytes, high_bytes);
	} else {
		value = getLong(popOperand(), 0x00000000);
	}

	field_class->static_vars[field_index].value = value;
}


void func_ret(){
	u2 index;

	frame_stack->frame->pc++;
	index = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if(wide) {
		frame_stack->frame->pc++;
		index = index << 8;
		index |= (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		wide = 0;
	}
	frame_stack->frame->pc = frame_stack->frame->local_variables[index];

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
		printf("\nNullPointerException at saload.\n");
		return;
	}
	if (index >= arrayref->size || index < 0){
		printf("\nArrayIndexOutOfBoundsException at faload.\n");
		return;
	}

	ref = (short *) arrayref->reference;
	memcpy(&stackValue, &ref[index], sizeof(short));

	pushOperand(stackValue);
}

void func_sastore(){
}

void func_sipush(){
	u2 byte1, byte2;
	u4 result;

	frame_stack->frame->pc++;
	byte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	byte2 = 0;
	byte2 |= (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	result = (byte1 << 8) | byte2;

	pushOperand(result);
}

void func_swap(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();

	pushOperand(value2);
	pushOperand(value1);
}


void func_tableswitch(){
	u4 defaultbyte1;
	u4 defaultbyte2;
	u4 defaultbyte3;
	u4 defaultbyte4;
	u4 lowbyte1;
	u4 lowbyte2;
	u4 lowbyte3;
	u4 lowbyte4;
	u4 highbyte1;
	u4 highbyte2;
	u4 highbyte3;
	u4 highbyte4;
	int default_value, low_value, high_value, value, j;
	int index;
	u4 target_adress;
	u4 offsets_count;
	int *offsets, i;

	frame_stack->frame->pc++;
	while (frame_stack->frame->pc %4 !=0){
		frame_stack->frame->pc++;
	}

	defaultbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	defaultbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	defaultbyte3 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	defaultbyte4 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	lowbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	lowbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	lowbyte3 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	lowbyte4 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	highbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	highbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	highbyte3 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	highbyte4 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;

	default_value = (defaultbyte1 << 24) | (defaultbyte2 << 16) | (defaultbyte3 << 8) | defaultbyte4;
	low_value = (lowbyte1 << 24) | (lowbyte2 << 16) | (lowbyte3 << 8) | lowbyte4;
	high_value = (highbyte1 << 24) | (highbyte2 << 16) | (highbyte3 << 8) | highbyte4;

	offsets_count = (high_value-low_value+1);
	offsets = malloc(sizeof(int)*offsets_count);

	for(i = 0; i < offsets_count; i++) {
		value = 0;
		for (j = 0; j < 3; j++) {
			value |=  frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
			value = value << 8;
			frame_stack->frame->pc++;
		}
		value |= frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;

		offsets[i] = value;
	}

	index = popOperand();
	if (index < low_value || index > high_value) {
		target_adress = offsets[default_value];
	}
	else {
		target_adress = offsets[index - low_value];
	}
	frame_stack->frame->pc = target_adress + 3;
}


void func_wide(){
	wide=1;
}


void func_breakpoint(){
	/* Não implementado */
}

void func_impdep1(){
}

void func_impdep2(){
}

