#ifndef GETPROCESSORCOREINFORMATION_HPP
#define GETPROCESSORCOREINFORMATION_HPP

#include <cstdint>
#include <optional>
#include <vector>

struct ProcessorCoreInformation {
public:

	explicit ProcessorCoreInformation(uint16_t efficiency_class, uint16_t count, bool has_hyperthreading) noexcept;

	uint16_t efficiency_class_;
	uint16_t count_;
	bool has_hyperthreading_;
};

std::optional<std::vector<ProcessorCoreInformation>> GetProcessorCoreInformation() noexcept;

#endif // #ifndef GETPROCESSORCOREINFORMATION_HPP