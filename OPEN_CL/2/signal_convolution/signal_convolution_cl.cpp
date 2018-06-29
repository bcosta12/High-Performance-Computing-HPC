#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
using namespace std;

int main(int argc, char ** argv) {
    const size_t N = 1 << atoi( argv[1] );
    ifstream t("kernel.cl");
    string source((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    try {
	// Get list of OpenCL platforms.
	std::vector<cl::Platform> platform;
	cl::Platform::get(&platform);

	if (platform.empty()) {
	    std::cerr << "OpenCL platforms not found." << std::endl;
	    return 1;
	}

	// Get first available GPU device which supports float precision.
	cl::Context context;
	std::vector<cl::Device> device;
	for(auto p = platform.begin(); device.empty() && p != platform.end(); p++) {
	    std::vector<cl::Device> pldev;

	    try {
		p->getDevices(CL_DEVICE_TYPE_CPU, &pldev);

		for(auto d = pldev.begin(); device.empty() && d != pldev.end(); d++) {
		    if (!d->getInfo<CL_DEVICE_AVAILABLE>()) continue;

		    std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();
		    device.push_back(*d);
		    context = cl::Context(device);
		}
	    } catch(...) {
		device.clear();
	    }
	}

	if (device.empty()) {
	    std::cerr << "GPUs with float precision not found." << std::endl;
	    return 1;
	}

	std::cout << device[0].getInfo<CL_DEVICE_NAME>() << std::endl;

	// Create command queue.
	cl::CommandQueue queue(context, device[0]);

	// Compile OpenCL program for found device.
	cl::Program program(context, cl::Program::Sources(
		    1, std::make_pair(source.c_str(), strlen(source.c_str()))
		    ));

	try {
	    program.build(device);
	} catch (const cl::Error&) {
	    std::cerr
		<< "OpenCL compilation error" << std::endl
		<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0])
		<< std::endl;
	    return 1;
	}

	cl::Kernel signal_convolution(program, "signal_convolution");

	// Prepare input data.
	std::vector<float> x( ( int ) 1e5, 1 );
	std::vector<float> s( ( int ) 1e5, 1 );
	std::vector<float> c( ( int ) 1e2, 1 );

	// Allocate device buffers and transfer input data to device.
	cl::Buffer X(context, CL_MEM_WRITE_ONLY,
		x.size() * sizeof(float), x.data());

	cl::Buffer S(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		s.size() * sizeof(float), s.data());

	cl::Buffer C(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		c.size() * sizeof(float), c.data());

	// Set kernel parameters.
	signal_convolution.setArg(0, X);
	signal_convolution.setArg(1, S);
	signal_convolution.setArg(2, C);
	
	// Launch kernel on the compute device.
	queue.enqueueNDRangeKernel(signal_convolution, cl::NullRange, N, cl::NullRange);

	// Get result back to host.
	queue.enqueueReadBuffer(X, CL_TRUE, 0, x.size() * sizeof(float), x.data());

	// Should get '3' here.
	std::cout << x[0] << std::endl;
    } catch (const cl::Error &err) {
	std::cerr
	    << "OpenCL error: "
	    << err.what() << "(" << err.err() << ")"
	    << std::endl;
	return 1;
    }
}
