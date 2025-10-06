#ifndef GETPROCESSORINFORMATION_HPP
#define GETPROCESSORINFORMATION_HPP

#include <optional>

class ProcessorCoreInformation {
public:
	int actual_cores_;
	int hyperthreading_cores_;
};

std::optional<ProcessorCoreInformation> GetProcessorInformation() noexcept;

#endif // #ifndef GETPROCESSORINFORMATION_HPP