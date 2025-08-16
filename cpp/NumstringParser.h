#ifndef NUMSTRINGPARSER_H
#define NUMSTRINGPARSER_H

#include <godot_cpp/classes/object.hpp>

#include "godot_cpp/core/binder_common.hpp"

class NumstringParser : public godot::Object {
	GDCLASS(NumstringParser, godot::Object)

public:
	enum NumberJumbleParseState {
		NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN,
		NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED,
		NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN,
		NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER,
		NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_NON_LEADING_FLOATING_POINT,
		NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT,
		NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_FLOATING_POINT,
		NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT,
		NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT_SIGN,
		NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_EXPONENT
	};

private:
	bool compress_numbers;

public:
	void set_compress_numbers(const bool p_compress_numbers) { compress_numbers = p_compress_numbers; }

	[[nodiscard]] bool get_compress_numbers() const { return compress_numbers; }

private:
	bool minimize_spacing;

public:
	void set_minimize_spacing(const bool p_minimize_spacing) { minimize_spacing = p_minimize_spacing; }

	[[nodiscard]] bool get_minimize_spacing() const { return minimize_spacing; }

public:
	[[nodiscard]] godot::String num_to_text(double p_num, bool p_is_angle = false) const;
	[[nodiscard]] godot::String numstr_arr_to_text(godot::PackedStringArray p_numstr_arr) const;

public:
	static void _bind_methods();

	static double evaluate(const godot::String &p_text);
	static godot::Array text_to_number_arr(const godot::String& p_text, int64_t p_current_index,
										   int64_t p_expected_count, bool allow_starting_comma = false);
	static godot::String basic_num_to_text(double p_num, bool p_is_angle = false);
};

VARIANT_ENUM_CAST(NumstringParser::NumberJumbleParseState)

#endif //NUMSTRINGPARSER_H
