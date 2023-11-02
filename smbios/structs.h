#pragma once

// Source: https://www.dmtf.org/sites/default/files/standards/documents/DSP0134_3.3.0.pdf

#ifndef SMBIOS_STRUCTS_H
#define SMBIOS_STRUCTS_H

#include "enums.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>

namespace fi {
	namespace smbios {
	#pragma pack(push, 1)
		struct smbios_t {
			std::uint8_t	calling_method;
			std::uint8_t	major_version;
			std::uint8_t	minor_verion;
			std::uint8_t	dmi_revision;
			std::uint32_t	length;
		};

		struct table_header_t {
			std::uint8_t	type;
			std::uint8_t	length;
			std::uint16_t	handle;
		};
		
		struct bios_information_t : public table_header_t {
			std::uint8_t	id_vendor;
			std::uint8_t	id_bios_version;
			std::uint16_t	bios_starting_address_segment;
			std::uint8_t	id_bios_release_date;
			std::uint8_t	bios_rom_size;
			std::uint64_t	bios_charateristics;
			std::uint16_t	bios_characteristics_extensions;
			std::uint8_t	bios_major_release;
			std::uint8_t	bios_minor_release;
			std::uint8_t	embedded_controller_fw_major_release;
			std::uint8_t	embedded_controler_fw_minor_release;
			std::uint16_t	extended_bios_rom_size;
		};

		struct smbios_uuid_t {
			std::uint32_t	time_low;
			std::uint16_t	time_mid;
			std::uint16_t	time_hi_and_version;
			std::uint8_t	clock_seq_hi_and_reversed;
			std::uint8_t	clock_seq_low;
			std::uint8_t	node[ 6 ];
		};

		struct system_information_t : public table_header_t {
			std::uint8_t	id_manufacturer;
			std::uint8_t	id_product_name;
			std::uint8_t	id_version;
			std::uint8_t	id_serial_number;
			smbios_uuid_t	uuid;
			std::uint8_t	wake_up_type;
			std::uint8_t	id_sku_number;
			std::uint8_t	id_family;
		};

		struct baseboard_information_t : public table_header_t {
			std::uint8_t	id_manufacturer;
			std::uint8_t	id_product;
			std::uint8_t	id_version;
			std::uint8_t	id_serial_number;
			std::uint8_t	id_asset_tag;
			baseboard_feature_flags	feature_flags;
			std::uint8_t	id_location_in_chassis;
			std::uint16_t	chassis_handle;
			baseboard_board_types	board_type;
			std::uint8_t	num_object_handles;
			std::uint16_t	object_handles[];
		};

		struct memory_device_t : public table_header_t {
			std::uint16_t physical_memory_array_handle;
			std::uint16_t memory_error_information_handle;
			std::uint16_t total_width;
			std::uint16_t data_width;
			std::uint16_t size;
			std::uint8_t form_factor;
			std::uint8_t device_set;
			std::uint8_t id_device_locator;
			std::uint8_t id_bank_locator;
			std::uint8_t memory_type;
			std::uint16_t type_detail;
			std::uint16_t speed;
			std::uint8_t id_manufacturer;
			std::uint8_t id_serial_number;
			std::uint8_t id_asset_tag;
			std::uint8_t id_part_number;
			std::uint8_t attributes;
			std::uint32_t extended_size;
			std::uint16_t configured_memory_speed;
			std::uint16_t minimum_voltage;
			std::uint16_t maximum_voltage;
			std::uint16_t configured_voltage;
			std::uint8_t memory_technology;
			std::uint16_t memory_operating_mode_capability;
			std::uint8_t id_firmware_version;
			std::uint16_t module_manufacturer_id;
			std::uint16_t module_product_id;
			std::uint16_t memory_subsystem;
			std::uint16_t memory_subsystem_controller_product_id;
			std::uint64_t non_volatile_size;
			std::uint64_t volatile_size;
			std::uint64_t cache_size;
			std::uint64_t logical_size;
			std::uint32_t extended_speed;
			std::uint32_t extended_configured_memory_speed;
		};
	#pragma pack(pop)

		enum table_types : std::uint8_t {
			bios_information = 0,
			system_information = 1,
			baseboard_information = 2,
			memory_device = 17,
			end_of_table = 127 // Has a lenght of 4
		};

		namespace detail {
			typedef std::uint16_t entry_handle;
			typedef std::unordered_map< std::uint8_t, std::string > table_string_container;

			struct smbios_table_entry_t {
				smbios_table_entry_t( ) { }

				smbios_table_entry_t( table_header_t* const table_header, entry_handle e_handle, const table_string_container& _table_strings ) : strings( _table_strings ), handle( e_handle ) { 
					formatted_section.assign( reinterpret_cast< std::uint8_t* >( table_header ), reinterpret_cast< std::uint8_t* >( table_header + table_header->length ) );
				}

				entry_handle handle = 0;
				table_string_container strings = { };
				std::vector< std::uint8_t > formatted_section = { };
			};

			template < typename T > 
			class smbios_table {
			public:
				smbios_table( ) { }
				smbios_table( const smbios_table_entry_t& entry ) : m_entry( entry ) { }

				std::string operator[]( int index ) {
					return m_entry.strings[ index ];
				}

				T* operator->( ) {
					return reinterpret_cast< T* >( m_entry.formatted_section.data( ) );
				}

			private:
				smbios_table_entry_t m_entry = { };
			};

		} // namespace detail
	} // namespace smbios
} // namespace fi

#endif // SMBIOS_STRUCT_H