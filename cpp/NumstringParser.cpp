#include "NumstringParser.h"

#include "godot_cpp/classes/expression.hpp"

using namespace godot;

template <char32_t... chars>
_FORCE_INLINE_ bool in(const char32_t ch) {
	return ((ch == chars) || ...);
}

template <typename T, T... possible_values>
_FORCE_INLINE_ bool has(const T ch) {
	return ((ch == possible_values) || ...);
}

_FORCE_INLINE_ bool inNum(const char32_t ch) { return in<'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'>(ch); }

_FORCE_INLINE_ bool inSpace(const char32_t ch) { return in<' ', '\t', '\n', '\r'>(ch); }

// Array NumstringParser::text_to_number_arr(const String& p_text, int64_t p_current_index, const int64_t
// p_expected_count, 										  const bool allow_starting_comma) {
// 	// print_line(p_text);
// 	// print_line(p_current_index);
// 	// print_line(p_expected_count);
// 	const int64_t text_length = p_text.length();
// 	if (p_current_index >= text_length) {
// 		return {};
// 	}
//
// 	NumberJumbleParseState state = allow_starting_comma ? NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED
// 														: NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
// 	int64_t current_number_start_idx = -1;
// 	auto parsed_numbers = PackedFloat64Array();
// 	while (true) {
// 		bool unrecognized_symbol = false;
// 		if (p_current_index == text_length) {
// 			unrecognized_symbol = true;
// 		} else {
// 			const char32_t current_char = p_text[p_current_index];
// 			switch (state) {
// 			case NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN:
// 				if (inSpace(current_char)) {
// 				} else if (inNum(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER;
// 					current_number_start_idx = p_current_index;
// 				} else if (in<'-', '+'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
// 					current_number_start_idx = p_current_index;
// 				} else if (current_char == '.') {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
// 					current_number_start_idx = p_current_index;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED:
// 				if (inSpace(current_char)) {
// 				} else if (current_char == ',') {
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
// 				} else if (inNum(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER;
// 					current_number_start_idx = p_current_index;
// 				} else if (in<'-', '+'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
// 					current_number_start_idx = p_current_index;
// 				} else if (current_char == '.') {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
// 					current_number_start_idx = p_current_index;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN:
// 				if (inNum(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER;
// 				} else if (current_char == '.') {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER:
// 				if (inNum(current_char)) {
// 				} else if (inSpace(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
// 					current_number_start_idx = -1;
// 				} else if (current_char == ',') {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
// 					current_number_start_idx = -1;
// 				} else if (in<'-', '+'>(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
// 					current_number_start_idx = p_current_index;
// 				} else if (current_char == '.') {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_NON_LEADING_FLOATING_POINT;
// 				} else if (in<'e', 'E'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_NON_LEADING_FLOATING_POINT:
// 				if (current_char == '.') {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
// 					current_number_start_idx = p_current_index;
// 				} else if (in<'-', '+'>(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
// 					current_number_start_idx = p_current_index;
// 				} else if (inNum(current_char))
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_FLOATING_POINT;
// 				else if (inSpace(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
// 					current_number_start_idx = -1;
// 				} else if (current_char == ',') {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
// 					current_number_start_idx = -1;
// 				} else if (in<'e', 'E'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT:
// 				if (in<'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_FLOATING_POINT;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_FLOATING_POINT:
// 				if (in<'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'>(current_char)) {
// 				} else if (current_char == '.') {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
// 					current_number_start_idx = p_current_index;
// 				} else if (in<' ', '\t', '\n', '\r'>(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
// 					current_number_start_idx = -1;
// 				} else if (current_char == ',') {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
// 					current_number_start_idx = -1;
// 				} else if (in<'-', '+'>(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
// 					current_number_start_idx = p_current_index;
// 				} else if (in<'e', 'E'>(current_char))
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT;
// 				else
// 					unrecognized_symbol = true;
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT:
// 				if (in<'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_EXPONENT;
// 				} else if (in<'-', '+'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT_SIGN;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT_SIGN:
// 				if (in<'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'>(current_char)) {
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_EXPONENT;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 				break;
// 			case NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_EXPONENT:
// 				if (in<'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'>(current_char)) {
// 				} else if (current_char == '.') {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
// 					current_number_start_idx = p_current_index;
// 				} else if (in<' ', '\t', '\n', '\r'>(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
// 					current_number_start_idx = -1;
// 				} else if (current_char == ',') {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
// 					current_number_start_idx = -1;
// 				} else if (in<'-', '+'>(current_char)) {
// 					parsed_numbers.append(
// 						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
// 					current_number_start_idx = p_current_index;
// 				} else {
// 					unrecognized_symbol = true;
// 				}
// 			}
// 		}
//
// 		if (unrecognized_symbol) {
// 			if (current_number_start_idx >= 0 && parsed_numbers.size() == p_expected_count - 1 &&
// 				not has<int, NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN,
// 						NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT,
// 						NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT,
// 						NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT_SIGN>(state)) {
// 				parsed_numbers.append(
// 					p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
// 				return {parsed_numbers, p_current_index};
// 			}
// 			if (current_number_start_idx < 0 && parsed_numbers.size() == p_expected_count)
// 				return {parsed_numbers, p_current_index};
// 			return {};
// 		}
//
// 		if (parsed_numbers.size() == p_expected_count)
// 			return {parsed_numbers, p_current_index};
//
// 		p_current_index++;
// 	}
// }
Array NumstringParser::text_to_number_arr(const String& p_text, int64_t p_current_index, const int64_t p_expected_count,
										  const bool allow_starting_comma) {
	// print_line(p_text);
	// print_line(p_current_index);
	// print_line(p_expected_count);
	const int64_t text_length = p_text.length();
	if (p_current_index >= text_length) {
		return {};
	}

	NumberJumbleParseState state = allow_starting_comma ? NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED
														: NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
	int64_t current_number_start_idx = -1;
	auto parsed_numbers = PackedFloat64Array();
	while (true) {
		bool unrecognized_symbol = false;
		if (p_current_index == text_length) {
			unrecognized_symbol = true;
		} else {
			auto current_char = p_text[p_current_index];
			if (state == NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN) {
				if (in<' ', '\t', '\n', '\r'>(current_char)) {
				} else if (inNum(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER;
					current_number_start_idx = p_current_index;
				} else if (in<'-', '+'>(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
					current_number_start_idx = p_current_index;
				} else if (current_char == '.') {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
					current_number_start_idx = p_current_index;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED) {
				if (in<' ', '\t', '\n', '\r'>(current_char)) {
				} else if (current_char == ',') {
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
				} else if (inNum(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER;
					current_number_start_idx = p_current_index;
				} else if (in<'-', '+'>(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
					current_number_start_idx = p_current_index;
				} else if (current_char == '.') {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
					current_number_start_idx = p_current_index;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN) {
				if (inNum(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER;
				} else if (current_char == '.') {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER) {
				if (inNum(current_char)) {
				} else if (in<' ', '\t', '\n', '\r'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
					current_number_start_idx = -1;
				} else if (current_char == ',') {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
					current_number_start_idx = -1;
				} else if (in<'-', '+'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
					current_number_start_idx = p_current_index;
				} else if (current_char == '.') {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_NON_LEADING_FLOATING_POINT;
				} else if (in<'e', 'E'>(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_NON_LEADING_FLOATING_POINT) {
				if (current_char == '.') {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
					current_number_start_idx = p_current_index;
				} else if (in<'-', '+'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
					current_number_start_idx = p_current_index;
				} else if (inNum(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_FLOATING_POINT;
				} else if (in<' ', '\t', '\n', '\r'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
					current_number_start_idx = -1;
				} else if (current_char == ',') {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
					current_number_start_idx = -1;
				} else if (in<'e', 'E'>(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT) {
				if (inNum(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_FLOATING_POINT;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_FLOATING_POINT) {
				if (inNum(current_char)) {
				} else if (current_char == '.') {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
					current_number_start_idx = p_current_index;
				} else if (in<' ', '\t', '\n', '\r'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
					current_number_start_idx = -1;
				} else if (current_char == ',') {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
					current_number_start_idx = -1;
				} else if (in<'-', '+'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
					current_number_start_idx = p_current_index;
				} else if (in<'e', 'E'>(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT) {
				if (inNum(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_EXPONENT;
				} else if (in<'-', '+'>(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT_SIGN;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT_SIGN) {
				if (inNum(current_char)) {
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_EXPONENT;
				} else {
					unrecognized_symbol = true;
				}
			} else if (state == NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_INDIRECTLY_AFTER_EXPONENT) {
				if (inNum(current_char)) {
				} else if (current_char == '.') {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT;
					current_number_start_idx = p_current_index;
				} else if (in<' ', '\t', '\n', '\r'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_ALLOWED;
					current_number_start_idx = -1;
				} else if (current_char == ',') {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_OUTSIDE_NUMBER_COMMA_FORBIDDEN;
					current_number_start_idx = -1;
				} else if (in<'-', '+'>(current_char)) {
					parsed_numbers.append(
						p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
					state = NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN;
					current_number_start_idx = p_current_index;
				} else {
					unrecognized_symbol = true;
				}
			}
		}
		if (unrecognized_symbol) {
			if (current_number_start_idx >= 0 && parsed_numbers.size() == p_expected_count - 1 &&
				not has<int, NUMBER_JUMBLER_PARSE_STATE_DIRECTLY_AFTER_SIGN,
						NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT,
						NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_LEADING_FLOATING_POINT,
						NUMBER_JUMBLER_PARSE_STATE_INSIDE_NUMBER_DIRECTLY_AFTER_EXPONENT_SIGN>(state)) {
				parsed_numbers.append(
					p_text.substr(current_number_start_idx, p_current_index - current_number_start_idx).to_float());
				return {parsed_numbers, p_current_index};
			}
			if (current_number_start_idx < 0 && parsed_numbers.size() == p_expected_count)
				return {parsed_numbers, p_current_index};
			return {};
		}

		if (parsed_numbers.size() == p_expected_count)
			return {parsed_numbers, p_current_index};
		p_current_index += 1;
	}
}

String NumstringParser::num_to_text(const double p_num, const bool p_is_angle) const {
	String text = String::num(p_num, p_is_angle ? 4 : 6).trim_suffix(".0");
	if (get_compress_numbers()) {
		if (text.begins_with("0.")) {
			text = text.right(-1);
		} else if (text.begins_with("-0.")) {
			text = text.erase(1);
		}
	}
	if (text == "-0")
		text = "0";
	return text;
}

String NumstringParser::numstr_arr_to_text(PackedStringArray p_numstr_arr) const {
	String output = "";
	for (int i = 0; i < p_numstr_arr.size() - 1; i++) {
		const String& current_numstr = p_numstr_arr[i];
		const char32_t next_char = p_numstr_arr[i + 1][0];
		output += current_numstr;
		if (!get_minimize_spacing() ||
			!((current_numstr.contains(".") && next_char == '.') || in<'-', '+'>(next_char))) {
			output += " ";
		}
	}
	return output + p_numstr_arr[p_numstr_arr.size() - 1];
}

void NumstringParser::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_minimize_spacing"), &NumstringParser::get_minimize_spacing);
	ClassDB::bind_method(D_METHOD("set_minimize_spacing", "minimize_spacing"), &NumstringParser::set_minimize_spacing);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "minimize_spacing", PROPERTY_HINT_NONE), "set_minimize_spacing",
				 "get_minimize_spacing");

	ClassDB::bind_method(D_METHOD("get_compress_numbers"), &NumstringParser::get_compress_numbers);
	ClassDB::bind_method(D_METHOD("set_compress_numbers", "compress_numbers"), &NumstringParser::set_compress_numbers);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "compress_numbers", PROPERTY_HINT_NONE), "set_compress_numbers",
				 "get_compress_numbers");

	ClassDB::bind_method(D_METHOD("num_to_text", "num", "is_angle"), &NumstringParser::num_to_text, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("numstr_arr_to_text", "numstr_arr"), &NumstringParser::numstr_arr_to_text);

	ClassDB::bind_static_method("NumstringParser", D_METHOD("evaluate", "text"), &NumstringParser::evaluate);
	ClassDB::bind_static_method(
		"NumstringParser",
		D_METHOD("text_to_number_arr", "text", "current_index", "expected_count", "allow_starting_comma"),
		&NumstringParser::text_to_number_arr, DEFVAL(false));
	ClassDB::bind_static_method("NumstringParser", D_METHOD("basic_num_to_text", "num", "is_angle"),
								&NumstringParser::basic_num_to_text, DEFVAL(false));
}

double NumstringParser::evaluate(const String& p_text) {
	const String new_text = p_text.strip_edges().trim_prefix("+");
	Expression* expr = memnew(Expression);
	Error err = expr->parse(new_text.replace(",", "."));
	if (err == OK) {
		if (auto result = expr->execute();
			!expr->has_execute_failed() && has<Variant::Type, Variant::FLOAT, Variant::INT>(result.get_type())) {
			return result;
		}
	}
	err = expr->parse(new_text);
	if (err == OK) {
		if (auto result = expr->execute();
			!expr->has_execute_failed() && has<Variant::Type, Variant::FLOAT, Variant::INT>(result.get_type())) {
			return result;
		}
	}
	return NAN;
}

String NumstringParser::basic_num_to_text(double p_num, bool p_is_angle) {
	String text = String::num(p_num, p_is_angle ? 4 : 6).trim_suffix(".0");
	if (text == "-0")
		text = "0";

	return text;
}
