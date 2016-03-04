
#include "Math.h"

namespace Jargon{
namespace Math{

	double lerp(double min, double max, double t){
		return min + (max - min) * t;
	}

	double rangeMap(double val, double sourceMin, double sourceMax, double destMin, double destMax){
		return (val - sourceMin) / (sourceMax - sourceMin) * (destMax - destMin) + destMin;
	}
}
}
