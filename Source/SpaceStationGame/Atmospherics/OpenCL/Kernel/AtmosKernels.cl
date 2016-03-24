__kernel void hello_world (__global char* message, int messageSize) {
	for (int i =0; i < messageSize; i++) {
		printf("%s", message[i]);
	}
}