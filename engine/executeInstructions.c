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
	u4 index, value, stackValue;
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
	    printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	ref = array->reference;
	ref = ref+index;
	memcpy(&stackValue, ref, sizeof(u4));
	pushOperand(stackValue);
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

	memcpy(&stackValue, array, sizeof(u4));
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
	reference = (u1 *) array_reference->reference;
	value = reference[index];
	pushOperand((u4) value);
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
	u4 objectref;
	u1 indexbyte1, indexbyte2;
	u2 index;
	cp_info resolved_class_type;

	objectref = popOperand();
	frame_stack->frame->pc++;
	indexbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = ((indexbyte1 << 8) | indexbyte2);

	resolved_class_type = getConstantPoolElementByIndexFromCurrentFrame(index);
	/*TODO se der tempo*/


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
	pushOperand(f_value);
}

void func_d2i(){
	u8 aux1;
	double aux2=0;
	u4 aux3, aux4;

	aux1 = (u8)popOperand();
	aux4 = popOperand();
	aux1 = aux1 << 32;
	aux1 |= aux4;
	memcpy(&aux2, &aux1, sizeof(double));


	aux3 = (u4)(aux2);
	pushOperand(aux3);
	frame_stack->frame->pc++;
}

void func_d2l(){
	u4 aux_value1, aux_value2;
	double double_value;
	u8 long_value;

	aux_value1 = popOperand();
	aux_value2 = popOperand();

	double_value = getDouble(aux_value2, aux_value1);
	long_value = (u8) double_value;

	aux_value1 = long_value >> 32;
	aux_value2 = long_value & 0xffffffff;
	pushOperand(aux_value2);	/*Gravo primeiro o low bytes*/
	pushOperand(aux_value1);	/*Depois o high bytes*/
}

void func_dadd(){
	u4 op1_hi, op1_low;
	u4 op2_hi, op2_low;
	u4 result_low, result_high;
	double op1, op2, result;

	op1_hi = popOperand();
	op1_low = popOperand();
	op2_hi = popOperand();
	op2_low = popOperand();

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
	double_value1 = getDouble(value2, value1);

	value1 = popOperand();
	value2 = popOperand();
	double_value2 = getDouble(value2, value1);

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
	u4 op1_hi = popOperand();
	u4 op1_low = popOperand();
	u4 op2_hi = popOperand();
	u4 op2_low = popOperand();
	int result;
	double op1, op2;

	op1 = getLong(op1_low, op1_hi);
	op2 = getLong(op2_low, op2_hi);

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
	double_value1 = getDouble(value2, value1);

	value1 = popOperand();
	value2 = popOperand();
	double_value2 = getDouble(value2, value1);

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
	u4 a1 = frame_stack->frame->local_variables[2];
	pushOperand(a1);
	a1 = frame_stack->frame->local_variables[1];
	pushOperand(a1);
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
	u4 hi,low;
	u4 a1 = 0xf;

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

    pushOperand(getDoubleHighBytes(d1));
    pushOperand(getDoubleLowBytes(d1));
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
	u4 value1_aux = popOperand();
	u4 value2_aux = popOperand();
	float value1, value2, result;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	result = value1+value2;

	memcpy(&result,&result,sizeof(u4));
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

	value1 = popOperand();
	value2 = popOperand();

	memcpy(&float_value1, &value1, sizeof(float));
	memcpy(&float_value2, &value2, sizeof(float));

	if (float_value1 > float_value2) {
		popOperand(1);
	} else {
		if (value1 < value2) {
			popOperand(-1);
		} else {
			popOperand(0);
		}
	}

}

void func_fcmpl(){
	u4 value1_aux = popOperand();
	u4 value2_aux = popOperand();
	u4 result;
	float value1, value2;

	memcpy(&value1, &value1_aux, sizeof(u4));
	memcpy(&value2, &value2_aux, sizeof(u4));

	if (value2 > value1) {
		result = 1;
	}
	else if(value2 == value1){
		result = 0;
	}
	else
		result = -1;
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

	value1 = popOperand();
	value2 = popOperand();

	memcpy(&float_value1, &value1, sizeof(float));
	memcpy(&float_value2, &value2, sizeof(float));

	result = float_value1 - float_value2;

	memcpy(&value1, &result, sizeof(u4));
	pushOperand(result);	/* Gravo o endereço para o float */
}


void func_getfield(){
}

void func_getstatic(){
	u4 indexbyte1, indexbyte2;
	u4 index;
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
	class_name = (char *)getConstanPoolElement(field.constant_union.c_class.name_index).constant_union.c_utf8.bytes;
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

	value = (char)popOperand();
	pushOperand((u4) value);

}

void func_i2c(){
	u4 value;

	value = (u1)popOperand();
	value &= 0x000000ff;
	pushOperand(value);
}

void func_i2d(){
	u4 a1, hi,low;
	double d;

	a1 = popOperand();
    d = (double)a1;

    hi = getDoubleHighBytes(d);
    low = getDoubleLowBytes(d);

    pushOperand(low);
    pushOperand(hi);
}

void func_i2f(){
	u4 value;
	float *result;

	result = malloc(sizeof(float));
	value = popOperand();
	*result = (float) value;
	pushOperand((u4) result);
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
	u4 index, *array_reference;

	index = popOperand();
	array_reference = (u4 *)popOperand();
	if (array_reference) {
		printf("NullPointerException!\n");
		exit(0);
	}
	pushOperand(array_reference[index]);
}

void func_iand(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();
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
	u4 *value1, *value2;
	u2 branchbyte1, branchbyte2, offset;


	value1 = (u4 *)popOperand();
	value2 = (u4 *)popOperand();
	if (*value1 == *value2) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_if_acmpne(){
	u4 *value1, *value2;
	u2 branchbyte1, branchbyte2, offset;


	value1 = (u4 *)popOperand();
	value2 = (u4 *)popOperand();
	if (*value1 != *value2) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_if_icmpeq(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	if (value1 == value2){
		frame_stack->frame->pc++;
		branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
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
	int value1, value2;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();
	value2 = (int) popOperand();

	if (value1 < value2) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_if_icmpge(){
	int value1, value2;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();
	value2 = (int) popOperand();

	if (value1 >= value2) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_if_icmpgt(){
	u4 value1 = popOperand();
	u4 value2 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	if (value1 < value2){
		frame_stack->frame->pc++;
		branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
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
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	if (value1 == 0) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_ifne(){
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	if (value1 != 0) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_iflt(){
	u4 value1 = popOperand();
	u2 branchbyte1, branchbyte2;
	short offset;

	if (value1 > 0){
		frame_stack->frame->pc++;
		branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
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
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	if (value1 > 0) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_ifle(){
	int value1;
	u2 branchbyte1, branchbyte2, offset;

	value1 = (int) popOperand();

	if (value1 <= 0) {
		frame_stack->frame->pc++;
		branchbyte1 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = (signed)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8) | branchbyte2;
		frame_stack->frame->pc += offset - 2;
	}
}

void func_ifnonnull(){
	u4 *value1;
	u2 branchbyte1, branchbyte2;
	short offset;

	value1 = (u4 *) popOperand();
	if (value1 != NULL){
		frame_stack->frame->pc++;
		branchbyte1 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		frame_stack->frame->pc++;
		branchbyte2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		offset = (branchbyte1 << 8)|branchbyte2;
		frame_stack->frame->pc += offset-1;
	}
}

void func_ifnull(){
	u4 *value;
	u4 brenchbyte1, brenchbyte2;
	short offset;

	value = (u4 *) popOperand();

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
}

void func_invokeinterface(){
	/*Não implementado pois os veteranos não implementaram. =D */
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
	long long aux1, aux3, aux4;
	double aux2;
	u4 aux;

	aux4 = popOperand();
	aux1 = popOperand();
	aux1 = aux1 << 32;
	aux1 |= aux4;
	aux2 = (double)aux1;
	memcpy(&aux3, &aux2, sizeof(int64_t));
	aux = aux3 >> 32;
	pushOperand(aux);
	aux = aux3 & 0xffffffff;
	pushOperand(aux);
}

void func_l2f(){
	u4 aux_value1, aux_value2;
	u8 long_value;
	float result;

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	long_value = getLong(aux_value2, aux_value1);

	result = (float) long_value;

	memcpy(&aux_value1, &result, sizeof(u4));

	pushOperand(aux_value1);
}

void func_l2i(){
	u4 value = popOperand();

	popOperand();
	pushOperand(value);
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
	u4 value1, value2, aux_value1, aux_value2;
	u8 result;

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value2 = getLong(aux_value2, aux_value1);

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 & value2;
	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongHighBytes(result);

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
	pushOperand(0);
	pushOperand(1);
}

void func_ldc(){
	u1 index;
	/*cp_info cp_string;*/

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];

	if (frame_stack->frame->cp->tag == 3){
		pushOperand(frame_stack->frame->cp[index].constant_union.c_integer.bytes);
	}
	else if ((frame_stack->frame->cp->tag == 4)){
		pushOperand(frame_stack->frame->cp[index].constant_union.c_float.bytes);
	}
	else if ((frame_stack->frame->cp->tag == 8)){
		/*cp_string = getConstantPoolElementByIndexFromCurrentFrame();*/
		pushOperand(frame_stack->frame->cp[index].constant_union.c_string.string_index);
	}
	else{
		printf("\n\t Error\n");
		return;
	}
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
			printf("\n ldc_w ERROR\n");
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
			printf("\n ldc_w ERROR\n");
		break;
	}
}

void func_ldiv(){
	u4 value1, value2, aux_value1, aux_value2;
	u8 result;

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value2 = getLong(aux_value2, aux_value1);

	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 / value2;
	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongHighBytes(result);

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

	while ((!found) || (i < number_pairs)) {
		if (key == pair->match) {
			found = 1;
			frame_stack->frame->pc = pair->offset + opcode;
		} else {
			pair++;
			i++;
		}
	}

	if(!found) {
		frame_stack->frame->pc = default_byte + opcode;
	}
}

void func_lor(){
	u4 valor1_hi = popOperand();
	u4 valor1_low = popOperand();
	u4 valor2_hi = popOperand();
	u4 valor2_low = popOperand();
	u4 result_aux;
	u8 valor1, valor2, result;

	valor1 = getLong(valor1_low, valor1_hi);
	valor2 = getLong(valor2_low, valor2_hi);

	result = valor1 | valor2;

	result_aux = result & 0xFFFFFFFF;
	pushOperand(result_aux);
    result_aux = result >> 32;
    pushOperand(result_aux);
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
	u4 value2, aux_value1, aux_value2;
	u8 value1, result;

	value2 = popOperand();
	aux_value1 = popOperand();
	aux_value2 = popOperand();
	value1 = getLong(aux_value2, aux_value1);

	result = value1 ^ value2;

	aux_value1 = getLongHighBytes(result);
	aux_value2 = getLongHighBytes(result);

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
		case 5:
			array->reference = calloc(count, sizeof(char));
		case 6:
			array->reference = calloc(count, sizeof(float));
		case 7:
			array->reference = calloc(count, sizeof(double));
		case 8:
			array->reference = calloc(count, sizeof(unsigned char));
		case 9:
			array->reference = calloc(count, sizeof(short int));
		case 10:
			array->reference = calloc(count, sizeof(int));
		case 11:
			array->reference = calloc(count, sizeof(long long));
	}

	memcpy(&stackValue, array, sizeof(u4));
	pushOperand(stackValue);
}

void func_nop(){
}


void func_pop(){
	popOperand();
}

void func_pop2(){
	popOperand();
	popOperand();
}

void func_putfield(){
	u4 indexbyte1, indexbyte2;
	u4 index;
	/*u4 *objectref;*/

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
		printf("\nNullPointerException at faload.\n");
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

