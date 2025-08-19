extends PanelContainer


signal selected
signal remove_requested
signal texture_changed
signal texture_size_changed


@onready var select_button: Button = %SelectButton
@onready var texture_rect: TextureRect = %TextureRect
@onready var number_edit = %NumberEdit
@onready var remove_button: Button = %RemoveButton


var texture: Texture2D


var svg_markup: String:
	set(value):
		svg_markup = value
		_update_texture()
var texture_size: int:
	set(value):
		texture_size = value
		if number_edit:
			number_edit.set_value(texture_size, false)
		_update_texture()
		texture_size_changed.emit()


func _ready() -> void:
	select_button.pressed.connect(selected.emit)
	remove_button.pressed.connect(remove_requested.emit)
	number_edit.set_value(texture_size, false)
	number_edit.value_changed.connect(func(new_value: float) -> void:
		texture_size = new_value as int
	)
	_update_texture()


func _update_texture() -> void:
	if not texture_rect:
		return
	var tex := SVGTexture.create_from_string(svg_markup)
	var tex_size := _get_tex_scale(Vector2i(tex.get_size()))
	if tex_size <= 0.0:
		tex_size = 0.01
	tex.base_scale = tex_size
	texture_rect.texture = tex
	texture_changed.emit()
	texture = tex
	#number_edit.suffix = "(x%.1f)" % tex_size


func _get_tex_scale(default_size: Vector2i) -> float:
	var max_dim_size := texture_size as int
	
	return max_dim_size / float(default_size[default_size.max_axis_index()])
