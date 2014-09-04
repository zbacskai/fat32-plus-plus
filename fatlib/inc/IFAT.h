#ifndef IFAT_h
#define IFAT_h

#include <fstream>

class IFAT {
protected:
	std::fstream &disk_;
public:
	class NextCluster {
	private:
		unsigned int cl_;
		bool valid_;
	public:
		NextCluster(unsigned int c, bool isValid) : cl_(c), valid_(isValid) {};
		NextCluster(const NextCluster &nc) : cl_(nc.cl_), valid_(nc.valid_) {};
		bool isValid() { return valid_; };
		operator unsigned int(){ return cl_; };
		void readCluster(unsigned char* buffer);
	};
	IFAT(std::fstream &disk) : disk_(disk){};
	virtual void printFAT() const = 0;
	virtual NextCluster getNextCluster(unsigned int clusterNumber) const = 0;
	virtual NextCluster getNextFreeCluster(unsigned int clusterNumber) const = 0;
	virtual void readCluster(const int clusterNumber, char* buffer) = 0;
	virtual unsigned int getClusterSizeInBytes() = 0;
	virtual ~IFAT() {};
};

#endif
