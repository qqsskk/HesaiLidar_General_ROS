#pragma once

#include <array>
#include "hesai_pandar/calibration.h"
#include "packet_decoder.h"
#include "pandar40.h"

namespace pandar40
{
class Pandar40Decoder : public PacketDecoder
{
public:
  enum class ReturnMode : int8_t {
    DUAL,
    FIRST,
    SECOND,
  };
  
  Pandar40Decoder(
    Calibration & calibration, float scan_phase = 0.0f, ReturnMode return_mode = ReturnMode::DUAL);
  void unpack(const pandar_msgs::PandarPacket & raw_packet) override;
  bool hasScanned() override;
  PointcloudXYZIRADT getPointcloud() override;

private:
  bool parsePacket(const pandar_msgs::PandarPacket & raw_packet);
  PointcloudXYZIRADT convert(int block_id);
  PointcloudXYZIRADT convert_dual(int block_id);

  std::array<float, LASER_COUNT> elev_angle_;
  std::array<float, LASER_COUNT> azimuth_offset_;

  std::array<float, LASER_COUNT> firing_offset_;
  std::array<float, BLOCKS_PER_PACKET> block_offset_single_;
  std::array<float, BLOCKS_PER_PACKET> block_offset_dual_;

  std::array<size_t, LASER_COUNT> firing_order_;

  ReturnMode return_mode_;
  Packet packet_;

  PointcloudXYZIRADT scan_pc_;
  PointcloudXYZIRADT overflow_pc_;

  uint16_t scan_phase_;
  int last_phase_;
  bool has_scanned_;
};

}  // namespace pandar40