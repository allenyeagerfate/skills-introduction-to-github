#ifndef SUB_PUB_H
#define SUB_PUB_H

#include <string>

#define PROPERTY_READ "/properties/read"
#define PROPERTY_REPLY "/property/read/reply"
#define FUNC_INVOKE "/function/invoke"
#define FUNC_REPLY "/func/reply"
#define LIST_READ "/properties/read/list"
#define LIST_REPLY "list/property/list/reply"
#define EVENT_REPLY "/event/reply"

const std::string productCodes[]={
	"no_azimuth_thruster_system",
	"power_system",
	"main_diesel_generator",
	"emergency_generator",
	"air_conditioner_refrigeration_system",
	"compressed_air_system",
	"fuel_oil_purifier_control_panelengine_room",
	"general_charger_and_discharger_panel",
	"main_transformer",
	"bow_thruster",
	"e_r_charger_and_discharger_panel",
	"refrigerate_provision_plant_control_box",
	"f_w_gen_cb",
	"water_sprinkler_alarm_control_box",
	"sewage_treatment_plant_control_box",
	"co2_discharge_alarm_system",
	"oil_water_separator_control_box",
	"level_gauging",
	"audio_visual_alarm_column_relay_box",
	"fire_alarm_control_unit",
	"public_address_general_alarm_system",
	"submersible_pump",
	"quick_closing_valve_control_system",
	"gas_detection_alarm_system_control_unit",
	"miscellaneous",
	"engineer_alarm_main_unit",
};


const std::string ams_prop_subs[]={
	"/no_azimuth_thruster_system/no_azimuth_thruster_system_device1/properties/read",
	"/no_azimuth_thruster_system/no_azimuth_thruster_system_device2/properties/read",
	"/no_azimuth_thruster_system/no_azimuth_thruster_system_device3/properties/read",
	"/power_system/power_system_device1/properties/read",
    "/em_cy_switchboard/em_cy_switchboard_device1/properties/read",
    "/main_diesel_generator/main_diesel_generator_device1/properties/read",
    "/main_diesel_generator/main_diesel_generator_device2/properties/read",
    "/main_diesel_generator/main_diesel_generator_device3/properties/read",
    "/main_diesel_generator/main_diesel_generator_device4/properties/read",
    "/main_diesel_generator/main_diesel_generator_device5/properties/read",
    "/main_diesel_generator/main_diesel_generator_device6/properties/read",
    "/emergency_generator/emergency_generator_device1/properties/read",
    "/air_conditioner_refrigeration_system/air_conditioner_refrigeration_system_device1/properties/read",
    "/compressed_air_system/compressed_air_system_device1/properties/read",
    "/fuel_oil_purifier_control_panelengine_room/fuel_oil_purifier_control_panelengine_room_device1/properties/read",
    "/general_charger_and_discharger_panel/general_charger_and_discharger_panel_device1/properties/read",
    "/main_transformer/main_transformer_device1/properties/read",
    "/bow_thruster/bow_thruster_device1/properties/read",
    "/e_r_charger_and_discharger_panel/e_r_charger_and_discharger_panel_device1/properties/read",
    "/refrigerate_provision_plant_control_box/refrigerate_provision_plant_control_box_device1/properties/read",
    "/f_w_gen_cb/f_w_gen_cb_device1/properties/read",
    "/water_sprinkler_alarm_control_box/water_sprinkler_alarm_control_box_device1/properties/read",
    "/sewage_treatment_plant_control_box/sewage_treatment_plant_control_box_device1/properties/read",
    "/co2_discharge_alarm_system/co2_discharge_alarm_system_device1/properties/read",
    "/oil_water_separator_control_box/oil_water_separator_control_box_device1/properties/read",
    "/level_gauging/level_gauging_device1/properties/read",
    "/audio_visual_alarm_column_relay_box/audio_visual_alarm_column_relay_box_device1/properties/read",
    "/fire_alarm_control_unit/fire_alarm_control_unit_device1/properties/read",
    "/public_address_general_alarm_system/public_address_general_alarm_system_device1/properties/read",
    "/submersible_pump/submersible_pump_device1/properties/read",
    "/quick_closing_valve_control_system/quick_closing_valve_control_system_device1/properties/read",
    "/gas_detection_alarm_system_control_unit/gas_detection_alarm_system_control_unit_device1/properties/read",
    "/miscellaneous/miscellaneous_device1/properties/read",
    "/neer_alarm_main_unit/engineer_alarm_main_unit_device1/properties/read",
};


const std::string ams_func_subs[]={
	"/no_azimuth_thruster_system/no_azimuth_thruster_system_device1/function/invoke",
	"/no_azimuth_thruster_system/no_azimuth_thruster_system_device2/function/invoke",
	"/no_azimuth_thruster_system/no_azimuth_thruster_system_device3/function/invoke",
	"/power_system/power_system_device1/function/invoke",
    "/em_cy_switchboard/em_cy_switchboard_device1/function/invoke",
    "/main_diesel_generator/main_diesel_generator_device1/function/invoke",
    "/main_diesel_generator/main_diesel_generator_device2/function/invoke",
    "/main_diesel_generator/main_diesel_generator_device3/function/invoke",
    "/main_diesel_generator/main_diesel_generator_device4/function/invoke",
    "/main_diesel_generator/main_diesel_generator_device5/function/invoke",
    "/main_diesel_generator/main_diesel_generator_device6/function/invoke",
    "/emergency_generator/emergency_generator_device1/function/invoke",
    "/air_conditioner_refrigeration_system/air_conditioner_refrigeration_system_device1/function/invoke",
    "/compressed_air_system/compressed_air_system_device1/function/invoke",
    "/fuel_oil_purifier_control_panelengine_room/fuel_oil_purifier_control_panelengine_room_device1/function/invoke",
    "/general_charger_and_discharger_panel/general_charger_and_discharger_panel_device1/function/invoke",
    "/main_transformer/main_transformer_device1/function/invoke",
    "/bow_thruster/bow_thruster_device1/function/invoke",
    "/e_r_charger_and_discharger_panel/e_r_charger_and_discharger_panel_device1/function/invoke",
    "/refrigerate_provision_plant_control_box/refrigerate_provision_plant_control_box_device1/function/invoke",
    "/f_w_gen_cb/f_w_gen_cb_device1/function/invoke",
    "/water_sprinkler_alarm_control_box/water_sprinkler_alarm_control_box_device1/function/invoke",
    "/sewage_treatment_plant_control_box/sewage_treatment_plant_control_box_device1/function/invoke",
    "/co2_discharge_alarm_system/co2_discharge_alarm_system_device1/function/invoke",
    "/oil_water_separator_control_box/oil_water_separator_control_box_device1/function/invoke",
    "/level_gauging/level_gauging_device1/function/invoke",
    "/audio_visual_alarm_column_relay_box/audio_visual_alarm_column_relay_box_device1/function/invoke",
    "/fire_alarm_control_unit/fire_alarm_control_unit_device1/function/invoke",
    "/public_address_general_alarm_system/public_address_general_alarm_system_device1/function/invoke",
    "/submersible_pump/submersible_pump_device1/function/invoke",
    "/quick_closing_valve_control_system/quick_closing_valve_control_system_device1/function/invoke",
    "/gas_detection_alarm_system_control_unit/gas_detection_alarm_system_control_unit_device1/function/invoke",
    "/miscellaneous/miscellaneous_device1/function/invoke",
    "/neer_alarm_main_unit/engineer_alarm_main_unit_device1/function/invoke",
};



#endif
