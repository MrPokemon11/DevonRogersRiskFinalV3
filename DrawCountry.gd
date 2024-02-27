extends Node2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var radius = 50

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

#should hopefully draw a circle
func _draw():
	var center = get_viewport().get_mouse_position()

	var color = Color(1.0,0.0,0.0)
	draw_circle(center, radius, color)
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	global_position = get_global_mouse_position()
	update()
