__kernel void square( __global float* Input, __global float* Output, int count)
{
	int i = get_global_id(0);
	if(i < count)
	{
		Output[i] = Input[i] * Input[i];
	}
}

