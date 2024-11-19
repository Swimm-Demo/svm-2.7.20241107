/*
 * Simple Virtual Machine - A versatile and robust architecture to
 * easily write applications.
 * Copyright (C) 2021  Julien BRUGUIER
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

function svm_create_div(svm_location,svm_class)
{
	var e = document.createElement('div');
	e.classList.add(svm_class);
	svm_location.appendChild(e);
	return e;
}
function svm_create_inline_div(svm_location,svm_class)
{
	var e = svm_create_div(svm_location,svm_class);
	e.style.display = 'inline';
	return e;
}
function svm_create_inline_block_div(svm_location,svm_class)
{
	var e = svm_create_div(svm_location,svm_class);
	e.style.display = 'inline-block';
	return e;
}

class SVM_Events
{
	constructor(svm_create, svm_destroy, svm_focus, svm_show, svm_hide, svm_notify, svm_acknowledge, svm_close)
	{
		this.svm_create = svm_create;
		this.svm_destroy = svm_destroy;
		this.svm_focus = svm_focus;
		this.svm_hide = svm_hide;
		this.svm_show = svm_show;
		this.svm_notify = svm_notify;
		this.svm_acknowledge = svm_acknowledge;
		this.svm_close = svm_close;
	}
}

class SVM_PositionFixed
{
	constructor(x, y)
	{
		this.x = x;
		this.y = y;
		this.relative = true;
	}
}

class SVM_PositionClever
{
	constructor(svm_desktop,svm_size_x,svm_size_y,svm_box)
	{
		var svm_position = svm_desktop.svm_clever_position(svm_size_x,svm_size_y,this);
		if(svm_position.x == null) { this.x = svm_desktop.svm_middle_x() - svm_size_x/2; } else { this.x = svm_position.x; }
		if(svm_position.y == null) { this.y = svm_desktop.svm_middle_y() - svm_size_y/2; } else { this.y = svm_position.y; }
		this.relative = true;
	}
}

class SVM_PositionBox
{
	constructor(svm_box)
	{
		this.x = svm_box.svm_position.x + 30;
		this.y = svm_box.svm_position.y + 30;
		this.relative = false;
	}
}

class SVM_SizeFixed
{
	constructor(x, y)
	{
		this.x = x;
		this.y = y;
	}
}

class SVM_SizeNatural
{
	constructor()
	{
	}
}

class SVM_SizeMinimum
{
	constructor(x,y)
	{
		this.x_min = x;
		this.y_min = y;
	}
}

class SVM_SizeMaximum
{
	constructor(x,y)
	{
		this.x_max = x;
		this.y_max = y;
	}
}

class SVM_SizeBounded
{
	constructor(x_min,y_min,x_max,y_max)
	{
		this.x_min = x_min;
		this.y_min = y_min;
		this.x_max = x_max;
		this.y_max = y_max;
	}
}

class SVM_SizeFixedWidthMinimumHeight
{
	constructor(x,y)
	{
		this.x = x;
		this.y_min = y;
	}
}

class SVM_Window
{
	constructor(svm_window,svm_desktop,svm_category,svm_title,svm_content,svm_events,svm_position,svm_size,svm_help)
	{
		this.dom = svm_window;
		this.dom.svm_window = this;
		this.svm_desktop = svm_desktop;
		this.svm_category = svm_category;
		this.svm_title = svm_title;
		this.svm_content = svm_content;
		this.svm_depth = this.svm_desktop.svm_new_depth();
		this.svm_help = svm_help;
		this.svm_update();
		this.svm_position = svm_position;
		if(svm_position.relative)
		{
			this.svm_position.x = this.svm_position.x+svm_desktop.dom.offsetLeft;
			this.svm_position.y = this.svm_position.y+svm_desktop.dom.offsetTop; 
		}
		this.svm_size = svm_size;
		this.svm_alarm = false;
		this.svm_visible = true;
		this.svm_lists = [];
		this.svm_events = svm_events;
		if(this.svm_events != null && this.svm_events.svm_create != null)
		{
			this.svm_events.svm_create(this);
		}
	}
	svm_update()
	{
		this.dom.classList.add(this.svm_category);
		this.dom.innerHTML = "";
		this.dom.style.position = 'fixed';
		var svm_box = svm_create_div(this.dom,'window_box');
		var svm_title_bar = svm_create_div(svm_box,'window_title_bar');
		var svm_title = svm_create_inline_block_div(svm_title_bar,'window_title');
		svm_title.innerHTML = this.svm_title;
		var svm_buttons = svm_create_inline_div(svm_title_bar,'window_buttons');
		var svm_alarm = svm_create_inline_div(svm_buttons,'window_buttons_alarm');
		svm_alarm.classList.add('window_button');
		if(this.svm_help != null)
		{
			var svm_help = svm_create_inline_div(svm_buttons,'window_buttons_help');
			svm_help.classList.add('window_button');
		}
		var svm_hide = svm_create_inline_div(svm_buttons,'window_buttons_hide');
		svm_hide.classList.add('window_button');
		var svm_close = svm_create_inline_div(svm_buttons,'window_buttons_close');
		svm_close.classList.add('window_button');
		var svm_content = svm_create_div(svm_box,'window_content');
		svm_content.innerHTML = this.svm_content;
		svm_alarm.svm_window = this;
		svm_hide.svm_window = this;
		svm_close.svm_window = this;
		if(this.svm_help != null)
		{
			svm_help.svm_window = this;
			svm_help.addEventListener('click',function(e) { e.target.svm_window.svm_display_help();});
		}
		svm_alarm.addEventListener('click',function(e) { e.target.svm_window.svm_acknowledge();});
		svm_hide.addEventListener('click',function(e) { e.target.svm_window.svm_hide();});
		svm_close.addEventListener('click',function(e) { e.target.svm_window.svm_close();});
		this.dom.addEventListener('click', function(e) { e.currentTarget.svm_window.svm_highlight(); }, true);
		this.dom.addEventListener('mousedown',
			function(e)
			{
				if(e.button!=1) return;
				var p = e.currentTarget.svm_window.svm_desktop.svm_nearest_position(e.currentTarget.svm_window,!e.ctrlKey);
				if(p.x==null) return;
				e.currentTarget.svm_window.svm_move(p.x,p.y);
			});
		this.dom.addEventListener('dragstart',
			function(e)
			{
				e.dataTransfer.setData('x',e.offsetX);
				e.dataTransfer.setData('y',e.offsetY);
				e.dataTransfer.setData('svm_window',e.currentTarget.svm_window.svm_index());
				e.currentTarget.svm_window.svm_highlight(); 
			});
	}
	svm_draw()
	{
		this.dom.style.left=this.svm_position.x;
		this.dom.style.top=this.svm_position.y;
		if(this.svm_size.x != null)
		{
			this.dom.childNodes[0].childNodes[1].style.width=this.svm_size.x;
		}
		if(this.svm_size.y != null)
		{
			this.dom.childNodes[0].childNodes[1].style.height=this.svm_size.y;
		}
		if(this.svm_size.x_min != null)
		{
			this.dom.childNodes[0].childNodes[1].style.minWidth= this.svm_size.x_min;
		}
		if(this.svm_size.x_max != null)
		{
			this.dom.childNodes[0].childNodes[1].style.maxWidth= this.svm_size.x_max;
		}
		if(this.svm_size.y_min != null)
		{
			this.dom.childNodes[0].childNodes[1].style.minHeight= this.svm_size.y_min;
		}
		if(this.svm_size.y_max != null)
		{
			this.dom.childNodes[0].childNodes[1].style.maxHeight= this.svm_size.y_max;
		}
		if(this.svm_alarm)
		{
			this.dom.childNodes[0].childNodes[0].childNodes[1].childNodes[0].classList.add('active');
		}
		else
		{
			this.dom.childNodes[0].childNodes[0].childNodes[1].childNodes[0].classList.remove('active');
		}
		if(this.svm_visible)
		{
			this.dom.style.display = 'block';
		}
		else
		{
			this.dom.style.display = 'none';
		}
		if(this.svm_focus)
		{
			this.dom.childNodes[0].childNodes[0].classList.add('focus');
		}
		else
		{
			this.dom.childNodes[0].childNodes[0].classList.remove('focus');
		}
		this.dom.style.zIndex=this.svm_depth;
		this.svm_desktop.svm_draw_windows_lists();
	}
	svm_notify()
	{
		this.svm_alarm = true;
		if(this.svm_events != null && this.svm_events.svm_notify != null)
		{
			this.svm_events.svm_notify(this);
		}
		this.svm_draw();
	}
	svm_acknowledge()
	{
		this.svm_alarm = false;
		if(this.svm_events != null && this.svm_events.svm_acknowledge != null)
		{
			this.svm_events.svm_acknowledge(this);
		}
		this.svm_draw();
	}
	svm_hide()
	{
		this.svm_visible = false;
		if(this.svm_events != null && this.svm_events.svm_hide != null)
		{
			this.svm_events.svm_hide(this);
		}
		this.svm_draw();
	}
	svm_show()
	{
		this.svm_visible = true;
		if(this.svm_events != null && this.svm_events.svm_show != null)
		{
			this.svm_events.svm_show(this);
		}
		this.svm_draw();
	}
	svm_close()
	{
		var f = true;
		if(this.svm_events != null && this.svm_events.svm_close != null)
		{
			f=this.svm_events.svm_close(this);
		}
		if(f)
		{
			this.svm_desktop.svm_remove_window(this);
		}
	}
	svm_highlight()
	{
		this.svm_desktop.svm_highlight(this);
	}
	svm_move(x,y)
	{
		this.svm_position.x = x;
		this.svm_position.y = y;
		this.svm_draw();
	}
	svm_index()
	{
		return this.svm_desktop.svm_index(this);
	}
	svm_add_windows_list(div)
	{
		for(var l of this.svm_lists)
		{
			if(l === div)
				return;
		}
		this.svm_lists.push(div);
		this.svm_draw_windows_lists();
	}
	svm_draw_windows_lists()
	{
		for(var l of this.svm_lists)
		{
			this.svm_desktop.svm_windows_list(l);
		}
	}
	svm_usable()
	{
		return this.dom.childNodes[0].childNodes[1];
	}
	svm_display_help()
	{
		this.svm_desktop.svm_add_window("help","<i>Help</i> - "+this.svm_title,this.svm_help,null,new SVM_PositionBox(this),new SVM_SizeMinimum(300,200));
	}
}

class SVM_Bar
{
	constructor(svm_bar)
	{
		this.dom = svm_bar;
		this.dom.svm_window = this;
		this.svm_depth = 0;
	}
	svm_update(svm_depth)
	{
		this.svm_depth = svm_depth;
	}
	svm_draw()
	{
		this.dom.style.zIndex = this.svm_depth;
	}
}

class SVM_Desktop
{
	constructor(svm_desktop,svm_unique)
	{
		this.dom = svm_desktop;
		this.dom.svm_window = this;
		this.dom.style.overflow = "hidden";
		this.dom.addEventListener('dragover', function(e) { e.preventDefault(); });
		this.dom.addEventListener('drop',
			function(e)
			{
				e.preventDefault();
				var x=e.dataTransfer.getData('x');
				var y=e.dataTransfer.getData('y');
				var svm_window=e.currentTarget.svm_window.svm_windows[e.dataTransfer.getData('svm_window')];
				svm_window.svm_move(e.pageX-x,e.pageY-y);
			});
		this.svm_windows = [];
		this.svm_bars = [];
		this.svm_unique = svm_unique;
	}
	svm_add_bar(svm_bar)
	{
		this.svm_bars.push(new SVM_Bar(svm_bar));
	}
	svm_add_window(svm_category,svm_title,svm_content,svm_events,svm_position,svm_size,svm_help)
	{
		if(this.svm_unique.indexOf(svm_category)!=-1)
		{
			var w = this.svm_windows.find(e => e.svm_category==svm_category);
			if(w != undefined)
			{
				w.svm_highlight();
				return w;
			}
		}
		var svm_window_dom = svm_create_div(this.dom,'window');
		svm_window_dom.draggable = true;
		var svm_window = new SVM_Window(svm_window_dom,this,svm_category,svm_title,svm_content,svm_events,svm_position,svm_size,svm_help);
		this.svm_windows.push(svm_window);
		svm_window.svm_highlight();
		svm_window.svm_draw();
		for(var b of this.svm_bars)
		{
			b.svm_update(svm_window.svm_depth+1);
			b.svm_draw();
		}
		this.svm_draw_windows_lists();
		return svm_window;
	}
	svm_highlight(svm_window)
	{
		var svm_maximum=0;
		for(var w of this.svm_windows)
		{
			var svm_depth = w.svm_depth;
			if(svm_depth>svm_maximum) { svm_maximum = svm_depth; }
			w.svm_focus = false;
		}
		for(var w of this.svm_windows)
		{
			if(w.svm_depth>svm_window.svm_depth)
			{
				w.svm_depth--;
			}
		}
		svm_window.svm_depth = svm_maximum;
		svm_window.svm_visible = true;
		svm_window.svm_focus = true;
		if(svm_window.svm_events != null && svm_window.svm_events.svm_focus != null)
		{
			svm_window.svm_events.svm_focus(svm_window);
		}
		this.svm_draw();
	}
	svm_remove_window(svm_window)
	{
		for(var i=0 ; i<this.svm_windows.length ; ++i)
		{
			if(this.svm_windows[i] === svm_window)
			{
				if(svm_window.svm_events != null && svm_window.svm_events.svm_destroy != null)
				{
					svm_window.svm_events.svm_destroy(svm_window);
				}
				this.dom.removeChild(this.svm_windows[i].dom);
				this.svm_windows.splice(i,1);
				break;
			}
		}
		this.svm_draw();
	}
	svm_index(svm_window)
	{
		for(var i=0 ; i<this.svm_windows.length ; ++i)
		{
			if(this.svm_windows[i] === svm_window)
			{
				return i;
			}
	
		}
		return null;
	}
	svm_windows_list(div)
	{
		div.innerHTML="";
		div.classList.add('windows_list');
		for(var svm_window of this.svm_windows)
		{
			var svm_window_title_bar = svm_create_div(div,'window_title_bar');
			svm_window_title_bar.classList.add('windows_list_window');
			svm_window_title_bar.svm_window = svm_window;
			var svm_title = svm_create_inline_div(svm_window_title_bar,'window_title_bar');
			svm_title.innerHTML = svm_window.svm_title;
			var svm_buttons = svm_create_inline_div(svm_window_title_bar,'window_buttons');
			var svm_alarm = svm_create_inline_div(svm_buttons,'window_buttons_alarm');
			svm_alarm.classList.add('window_button');
			if(svm_window.svm_alarm)
			{
				svm_alarm.classList.add('active');
			}
			svm_alarm.svm_window = svm_window;
			var svm_hide = svm_create_inline_div(svm_buttons,'window_buttons_hide');
			svm_hide.classList.add('window_button');
			svm_hide.svm_window = svm_window;
			var svm_close = svm_create_inline_div(svm_buttons,'window_buttons_close');
			svm_close.classList.add('window_button');
			svm_close.svm_window = svm_window;
			svm_window_title_bar.addEventListener('click',function(e) { e.currentTarget.svm_window.svm_highlight(); });
			svm_alarm.addEventListener('click',function(e) { e.currentTarget.svm_window.svm_acknowledge(); });
			svm_hide.addEventListener('click',function(e) { e.currentTarget.svm_window.svm_hide(); e.stopPropagation(); });
			svm_close.addEventListener('click',function(e) { e.currentTarget.svm_window.svm_close(); e.stopPropagation(); });
		}
	}
	svm_middle_x()
	{
		return this.dom.offsetLeft + this.dom.clientWidth/2;
	}
	svm_middle_y()
	{
		return this.dom.offsetTop + this.dom.clientHeight/2;
	}
	svm_new_depth()
	{
		var svm_maximum=0;
		for(var svm_window of this.svm_windows)
		{
			var svm_depth = svm_window.svm_depth;
			if(svm_depth>svm_maximum) { svm_maximum = svm_depth; }
		}
		var svm_minimum=svm_maximum;
		for(var svm_window of this.svm_windows)
		{
			var svm_depth = svm_window.svm_depth;
			if(svm_depth<svm_minimum) { svm_minimum = svm_depth; }
		}
		for(var svm_window of this.svm_windows)
		{
			svm_window.svm_depth -= svm_minimum;
		}
		return svm_maximum-svm_minimum+1;
	}
	svm_draw()
	{
		for(var svm_window of this.svm_windows)
		{
			svm_window.svm_draw();
		}
	}
	svm_draw_windows_lists()
	{
		for(var svm_window of this.svm_windows)
		{
			svm_window.svm_draw_windows_lists();
		}
	}
	svm_select(svm_category)
	{
		var svm_windows = [];
		for(var svm_window of this.svm_windows)
		{
			if(svm_window.svm_category == svm_category)
			{
				svm_windows.push(svm_window);
			}
		}
		return svm_windows;
	}
	svm_clever_position(svm_size_x,svm_size_y,svm_box)
	{
		var svm_space = 5;
		var svm_cursor_x = this.dom.offsetLeft+svm_space;
		var svm_cursor_y = this.dom.offsetTop+svm_space;
		
		while(svm_cursor_y+svm_size_y<=this.dom.offsetTop+this.dom.clientHeight-svm_space)
		{
			var svm_next_y = this.dom.clientHeight;
			while(svm_cursor_x+svm_size_x<=this.dom.offsetLeft+this.dom.clientWidth-svm_space)
			{
				var svm_intersect = false;
				for(var svm_window of this.svm_windows)
				{
					if((svm_window==svm_box)||(!svm_window.svm_visible)) { continue; }
					if(!((svm_cursor_x+svm_size_x<svm_window.dom.offsetLeft)||(svm_cursor_x>svm_window.dom.offsetLeft+svm_window.dom.offsetWidth)||(svm_cursor_y+svm_size_y<svm_window.dom.offsetTop)||(svm_cursor_y>svm_window.dom.offsetTop+svm_window.dom.offsetHeight)))
					{
						svm_intersect = true;
						svm_cursor_x = svm_window.dom.offsetLeft+svm_window.dom.offsetWidth+svm_space;
						if(svm_window.dom.offsetTop+svm_window.dom.offsetHeight<svm_next_y)
						{
							svm_next_y = svm_window.dom.offsetTop+svm_window.dom.offsetHeight;
						}
						break;
					}
				}
				if(!svm_intersect)
				{
					return { x: svm_cursor_x-this.dom.offsetLeft, y: svm_cursor_y-this.dom.offsetTop};
				}
			}
			svm_cursor_y = svm_next_y+svm_space;
			svm_cursor_x = svm_space;
		}
		return { x: null, y: null };
	}
	svm_nearest_position(svm_box,overlap)
	{
		var svm_space = 5;
		var svm_distance = 2*(Math.pow(this.dom.offsetWidth,2)+Math.pow(this.dom.offsetHeight,2));
		var svm_position = { x: null, y: null };
		var svm_handled = false;
		for(var p = 1 ; p<3 ; p++)
		{
			for(var c of this.svm_windows)
			{
				if((c==svm_box)||(!c.svm_visible)) { continue; }
				if(p==1)
				{
					var lp = [
						{ x: this.dom.offsetLeft+svm_space, y: this.dom.offsetTop+svm_space },
						{ x: this.dom.offsetLeft+this.dom.offsetWidth-svm_box.dom.offsetWidth-svm_space, y: this.dom.offsetTop+svm_space },
						{ x: this.dom.offsetLeft+svm_space, y: this.dom.offsetTop+this.dom.offsetHeight-svm_box.dom.offsetHeight-svm_space },
						{ x: this.dom.offsetLeft+this.dom.offsetWidth-svm_box.dom.offsetWidth-svm_space, y: this.dom.offsetTop+this.dom.offsetHeight-svm_box.dom.offsetHeight-svm_space },
						{ x: c.dom.offsetLeft, y: c.dom.offsetTop-svm_box.dom.offsetHeight-svm_space},
						{ x: c.dom.offsetLeft-svm_box.dom.offsetWidth-svm_space, y: c.dom.offsetTop },
						{ x: c.dom.offsetLeft+c.dom.offsetWidth+svm_space, y: c.dom.offsetTop },
						{ x: c.dom.offsetLeft, y: c.dom.offsetTop+c.dom.offsetHeight+svm_space },
						{ x: c.dom.offsetLeft+c.dom.offsetWidth-svm_box.dom.offsetWidth, y: c.dom.offsetTop-svm_box.dom.offsetHeight-svm_space },
						{ x: c.dom.offsetLeft-svm_box.dom.offsetWidth-svm_space, y: c.dom.offsetTop+c.dom.offsetHeight-svm_box.dom.offsetHeight },
						{ x: c.dom.offsetLeft+c.dom.offsetWidth+svm_space, y: c.dom.offsetTop+c.dom.offsetHeight-svm_box.dom.offsetHeight },
						{ x: c.dom.offsetLeft+c.dom.offsetWidth-svm_box.dom.offsetWidth, y: c.dom.offsetTop+c.dom.offsetHeight+svm_space }
						];
				} else if(p==2)
				{
					var bx = Math.min(Math.max(this.dom.offsetLeft+svm_space,svm_box.dom.offsetLeft),this.dom.offsetLeft+this.dom.offsetWidth-svm_box.dom.offsetWidth-svm_space);
					var by = Math.min(Math.max(this.dom.offsetTop+svm_space,svm_box.dom.offsetTop),this.dom.offsetTop+this.dom.offsetHeight-svm_box.dom.offsetHeight-svm_space);
					var lp = [
						{ x: bx, y: c.dom.offsetTop-svm_box.dom.offsetHeight-svm_space },
						{ x: c.dom.offsetLeft-svm_box.dom.offsetWidth-svm_space, y: by },
						{ x: c.dom.offsetLeft+c.dom.offsetWidth+svm_space, y: by },
						{ x: bx, y: c.dom.offsetTop+c.dom.offsetHeight+svm_space }
						];
				}
				for(var plp of lp)
				{
					if(((plp.x-svm_space)<this.dom.offsetLeft)||((plp.y-svm_space)<this.dom.offsetTop)||((plp.x+svm_space)>(this.dom.offsetLeft+this.dom.offsetWidth))||((plp.y+svm_space)>(this.dom.offsetTop+this.dom.offsetHeight))) { continue; }
					var svm_free = true;
					if(!overlap)
					{
						for(var svm_window of this.svm_windows)
						{
							if((svm_window==svm_box)||(!svm_window.svm_visible)) { continue; }
							svm_handled = true;
							if(!(((svm_window.dom.offsetLeft+svm_window.dom.offsetWidth)<=(plp.x-svm_space))||((plp.x+svm_box.dom.offsetWidth+svm_space)<=svm_window.dom.offsetLeft)||((svm_window.dom.offsetTop+svm_window.dom.offsetHeight)<=(plp.y-svm_space))||((plp.y+svm_box.dom.offsetHeight+svm_space)<=svm_window.dom.offsetTop)))
							{
								svm_free = false;
								break;
							}
						}
					}
					if(svm_free)
					{
						var d=Math.pow(svm_box.dom.offsetLeft-plp.x,2)+Math.pow(svm_box.dom.offsetTop-plp.y,2);
						if(d<svm_distance)
						{
							svm_distance=d;
							svm_position=plp;
						}
					}
				}
			}
			if(svm_position.x!=null) { break; }
		}
		if(!svm_handled)
		{
			var lp = [
				{ x: this.dom.offsetLeft+svm_space, y: this.dom.offsetTop+svm_space },
				{ x: this.dom.offsetLeft+this.dom.offsetWidth-svm_box.dom.offsetWidth-svm_space, y: this.dom.offsetTop+svm_space },
				{ x: this.dom.offsetLeft+svm_space, y: this.dom.offsetTop+this.dom.offsetHeight-svm_box.dom.offsetHeight-svm_space },
				{ x: this.dom.offsetLeft+this.dom.offsetWidth-svm_box.dom.offsetWidth-svm_space, y: this.dom.offsetTop+this.dom.offsetHeight-svm_box.dom.offsetHeight-svm_space } ];
			for(var plp of lp)
			{
				var d=Math.pow(svm_box.dom.offsetLeft-plp.x,2)+Math.pow(svm_box.dom.offsetTop-plp.y,2);
				if(d<svm_distance)
				{
					svm_distance=d;
					svm_position=plp;
				}
			}
		}
		return svm_position;
	}
}
