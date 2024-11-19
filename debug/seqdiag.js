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

class SVM_Participant
{
	constructor(id,svm_name,svm_horizontal_index)
	{
		this.id = id;
		this.svm_name = svm_name;
		this.svm_horizontal_index = svm_horizontal_index;
		this.svm_left_waiting_index = new Set();
		this.svm_right_waiting_index = new Set();

		var c = document.createElement("canvas");
		var cc = c.getContext("2d");
		this.svm_name_size = cc.measureText(svm_name).width;
		this.svm_left_event_max_size = 0;
		this.svm_right_event_max_size = this.svm_name_size;

		this.svm_vertical_position = null;
	}
};

class SVM_Event
{
	constructor(id,svm_event,svm_vertical_index,svm_start_participant,svm_end_participant,svm_start_space,svm_end_space,svm_event_size)
	{
		this.id = id;
		this.svm_event = svm_event;
		this.svm_start_index = svm_vertical_index;
		this.svm_start_participant = svm_start_participant;
		this.svm_end_index = null;
		this.svm_cancelled = false;
		this.svm_end_participant = svm_end_participant;
		this.svm_start_space = svm_start_space;
		this.svm_end_space = svm_end_space;

		this.svm_event_size = svm_event_size;
	}
};

class SVM_SequenceDiagramMarkers
{
	constructor(participants,events,color)
	{
		this.participants = participants;
		this.events = events;
		this.color = color;
	}
};

class SVM_SequenceDiagram
{
	constructor(canvas,detectors,display)
	{
		this.canvas = canvas;
		this.detectors = detectors;
		this.display = display;
		this.svm_vertical_index = 0;
		this.svm_participants = new Map();
		this.svm_events = new Map();
		this.svm_last_received_event_index = null;
		this.svm_waiting_answers = new Map();
		this.svm_event_markers = new Map();
		this.svm_participant_markers = new Map();

		this.svm_zoom = 1;
		this.svm_diagram_color = "#000000";
		this.svm_markers_color = "#DD0000";

		this.svm_start_space = 20;
		this.svm_participant_text_space = 5;
		this.svm_event_text_space = 5;
		this.svm_vertical_space = 20;
		this.svm_horizontal_space = 20;
		this.svm_space_space = 15;
		this.svm_participant_width = 3;
		this.svm_event_width = 1;
		this.svm_arrow_size = 5;
		this.svm_cross_size = 3;
		this.svm_canvas_size_advance = 1000;
		this.svm_detector_width = 5;
	}
	svm_add_event(e)
	{
		var svm_redraw = this.svm_compute_event(e);
		var svm_end = 2*this.svm_start_space+this.svm_vertical_space*(1+this.svm_vertical_index);
		if(svm_redraw||(svm_end>=this.canvas.height))
		{
			this.svm_diagram_draw();
		}
		else
		{
			this.svm_event_draw(e);
		}
	}
	svm_compute_event(e)
	{
		var svm_redraw = false;
		var svm_start_participant = this.svm_participants.get(e.source.id);
		if(svm_start_participant == undefined)
		{
			svm_start_participant = new SVM_Participant(e.source.id,e.source.name,this.svm_participants.size);
			this.svm_participants.set(e.source.id,svm_start_participant);
			svm_redraw = true;
		}
		var svm_end_participant = this.svm_participants.get(e.target.id);
		if(svm_end_participant == undefined)
		{
			svm_end_participant = new SVM_Participant(e.target.id,e.target.name,this.svm_participants.size);
			this.svm_participants.set(e.target.id,svm_end_participant);
			svm_redraw = true;
		}
		if(e.type=="post")
		{
			this.svm_vertical_index++;
			var c = document.createElement("canvas");
			var cc = c.getContext("2d");
			var svm_name_size = cc.measureText(e.event).width;
			var svm_extended_name_size = svm_name_size;
			var svm_end_space=0;
			var svm_start_space=0;
			if(svm_start_participant.svm_horizontal_index<svm_end_participant.svm_horizontal_index)
			{
				for(var i of svm_end_participant.svm_left_waiting_index)
				{
					svm_end_space = Math.max(svm_end_space,this.svm_events.get(i).svm_end_space);
				}
				for(var i of svm_start_participant.svm_right_waiting_index)
				{
					svm_start_space = Math.max(svm_start_space,this.svm_events.get(i).svm_end_space);
				}
				svm_extended_name_size += svm_start_space*this.svm_space_space
				svm_end_participant.svm_left_waiting_index.add(e.id);
				if(svm_start_participant.svm_right_event_max_size<svm_extended_name_size)
				{
					svm_start_participant.svm_right_event_max_size = svm_extended_name_size;
					svm_redraw = true;
				}
			}
			else
			{
				for(var i of svm_end_participant.svm_right_waiting_index)
				{
					svm_end_space = Math.max(svm_end_space,this.svm_events.get(i).svm_end_space);
				}
				for(var i of svm_start_participant.svm_left_waiting_index)
				{
					svm_start_space = Math.max(svm_start_space,this.svm_events.get(i).svm_end_space);
				}
				svm_extended_name_size += svm_start_space*this.svm_space_space
				svm_end_participant.svm_right_waiting_index.add(e.id);
				if(svm_start_participant.svm_left_event_max_size<svm_extended_name_size)
				{
					svm_start_participant.svm_left_event_max_size = svm_extended_name_size;
					svm_redraw = true;
				}
			}
			svm_end_space++;
			this.svm_events.set(e.id,new SVM_Event(e.id,e.event,this.svm_vertical_index,svm_start_participant,svm_end_participant,svm_start_space,svm_end_space,svm_name_size));
			this.svm_last_received_event_index = e.id;
			var r = this.svm_waiting_answers.get(e.id);
			if(r!=undefined)
			{
				this.svm_waiting_answers.delete(e.id);
				this.svm_compute_event(r);
			}
		}
		else if((e.type=="recv")||(e.type=="cancel"))
		{
			if((e.ref!=this.svm_last_received_event_index)||(e.source.id==e.target.id))
			{
				this.svm_vertical_index++;
			}
			this.svm_last_received_event_index = null;
			var ev = this.svm_events.get(e.ref);
			if(ev != undefined)
			{
				ev.svm_end_index = this.svm_vertical_index;
			}
			else
			{
				this.svm_waiting_answers.set(e.ref,e);
			}
			svm_end_participant.svm_left_waiting_index.delete(e.ref);
			svm_end_participant.svm_right_waiting_index.delete(e.ref);
			ev.svm_cancelled = (e.type=="cancel");
		}
		return svm_redraw;
	}
	svm_zoom_increase()
	{
		this.svm_zoom += 0.2;
		this.svm_diagram_draw();
	}
	svm_zoom_decrease()
	{
		if(this.svm_zoom>0.3)
		{
			this.svm_zoom -= 0.2;
			this.svm_diagram_draw();
		}
	}
	svm_zoom_reset()
	{
		this.svm_zoom = 1;
		this.svm_diagram_draw();
	}
	svm_marker(e)
	{
		var rect = this.canvas.getBoundingClientRect();
		var x = e.clientX-rect.left;
		var y = Math.trunc((e.clientY-rect.top-this.svm_start_space*this.svm_zoom)/(this.svm_vertical_space*this.svm_zoom));
		if(y>this.svm_vertical_index) { return; }
		if((e.clientY-rect.top)<this.svm_start_space*this.svm_zoom)
		{
			var participant = this.svm_participants.keys().next().value;
			for(var p of this.svm_participants)
			{
				if(((p[1].svm_vertical_position-2*this.svm_participant_text_space)*this.svm_zoom)<x)
				{
					participant = p[0];
				}
			}
			if(this.svm_participant_markers.has(participant))
			{
				this.svm_participant_markers.delete(participant);
			}
			else
			{
				this.svm_participant_markers.set(participant,this.svm_markers_color);
			}
		}
		else
		{
			if(this.svm_event_markers.has(y))
			{
				this.svm_event_markers.delete(y);
			}
			else
			{
				this.svm_event_markers.set(y,this.svm_markers_color);
			}
		}
		this.svm_diagram_draw();
	}
	svm_markers_reset()
	{
		this.svm_event_markers = new Map();
		this.svm_participant_markers = new Map();
		this.svm_diagram_draw();
	}
	svm_post_event_draw(ctx,e)
	{
		var d = (e.svm_end_participant.svm_vertical_position<e.svm_start_participant.svm_vertical_position)?-1:1;
		ctx.moveTo(e.svm_start_participant.svm_vertical_position,this.svm_start_space+(e.svm_start_index)*this.svm_vertical_space);
		ctx.lineTo(e.svm_end_participant.svm_vertical_position-d*e.svm_end_space*this.svm_space_space,this.svm_start_space+(e.svm_start_index)*this.svm_vertical_space);
	}
	svm_recv_event_draw(ctx,e)
	{
		var d = (e.svm_end_participant.svm_vertical_position<e.svm_start_participant.svm_vertical_position)?-1:1;
		ctx.moveTo(e.svm_end_participant.svm_vertical_position-d*e.svm_end_space*this.svm_space_space,this.svm_start_space+(e.svm_start_index)*this.svm_vertical_space);
		ctx.lineTo(e.svm_end_participant.svm_vertical_position-d*e.svm_end_space*this.svm_space_space,this.svm_start_space+(e.svm_end_index)*this.svm_vertical_space)
		var svm_end_x = e.svm_end_participant.svm_vertical_position-d*this.svm_participant_width;
		var svm_end_y = this.svm_start_space+(e.svm_end_index)*this.svm_vertical_space;
		ctx.lineTo(svm_end_x,svm_end_y);
		ctx.lineTo(svm_end_x-d*this.svm_arrow_size,svm_end_y-this.svm_arrow_size);
		ctx.moveTo(svm_end_x,svm_end_y);
		ctx.lineTo(svm_end_x-d*this.svm_arrow_size,svm_end_y+this.svm_arrow_size);
	}
	svm_cancel_event_draw(ctx,e)
	{
		var d = (e.svm_end_participant.svm_vertical_position<e.svm_start_participant.svm_vertical_position)?-1:1;
		ctx.moveTo(e.svm_end_participant.svm_vertical_position-d*e.svm_end_space*this.svm_space_space,this.svm_start_space+(e.svm_start_index)*this.svm_vertical_space);
		var svm_end_x = e.svm_end_participant.svm_vertical_position-d*e.svm_end_space*this.svm_space_space;
		var svm_end_y = this.svm_start_space+(e.svm_end_index)*this.svm_vertical_space;
		ctx.lineTo(svm_end_x,svm_end_y);
		ctx.moveTo(svm_end_x-this.svm_cross_size,svm_end_y-this.svm_cross_size);
		ctx.lineTo(svm_end_x+this.svm_cross_size,svm_end_y+this.svm_cross_size);
		ctx.moveTo(svm_end_x-this.svm_cross_size,svm_end_y+this.svm_cross_size);
		ctx.lineTo(svm_end_x+this.svm_cross_size,svm_end_y-this.svm_cross_size);
	}
	svm_text_event_draw(ctx,e)
	{
		var d = (e.svm_end_participant.svm_vertical_position<e.svm_start_participant.svm_vertical_position)?-1:1;
		if(d>0)
		{
			ctx.fillText(e.svm_event,e.svm_start_participant.svm_vertical_position+e.svm_start_space*this.svm_space_space+this.svm_event_text_space,this.svm_start_space+(e.svm_start_index)*this.svm_vertical_space-2);
		}
		else
		{
			ctx.fillText(e.svm_event,e.svm_start_participant.svm_vertical_position-e.svm_start_space*this.svm_space_space-this.svm_event_text_space-e.svm_event_size,this.svm_start_space+(e.svm_start_index)*this.svm_vertical_space-2);
		}
	}
	svm_markers_draw(ctx)
	{
		for(var m of this.svm_participant_markers)
		{
			var x = this.svm_participants.get(m[0]).svm_vertical_position;
			ctx.lineWidth = this.svm_participant_width;
			ctx.strokeStyle=m[1];
			ctx.beginPath();
			ctx.moveTo(x,this.svm_start_space);
			ctx.lineTo(x,this.svm_start_space+this.svm_vertical_space*(1+this.svm_vertical_index));
			ctx.stroke();
		}
		for(var m of this.svm_event_markers)
		{
			var y = this.svm_start_space+m[0]*this.svm_vertical_space+this.svm_arrow_size+1;
			ctx.strokeStyle=m[1];
			ctx.lineWidth = this.svm_event_width;
			ctx.beginPath();
			ctx.moveTo(0,y);
			ctx.lineTo(this.canvas.width,y);
			ctx.stroke();
		}
	}
	svm_detectors_draw()
	{
		this.detectors.innerHTML = "";
		this.display.innerHTML = "";

		for(var p of this.svm_participants)
		{
			var d = document.createElement('div');
			d.style.position = "absolute";
			d.style.top = Math.round(this.svm_start_space*this.svm_zoom);
			d.style.left = Math.round((p[1].svm_vertical_position - this.svm_detector_width - this.svm_participant_width/2)*this.svm_zoom);
			d.style.height = Math.round((this.svm_vertical_space*(1+this.svm_vertical_index))*this.svm_zoom);
			d.style.width = Math.round((2*this.svm_detector_width+this.svm_participant_width)*this.svm_zoom);
			d.svm_seqdiag = this;
			d.svm_name = p[1].svm_name;
			d.addEventListener('mouseenter',function(event) { event.target.svm_seqdiag.display.innerText = event.target.svm_name; });
			d.addEventListener('mouseleave',function(event) { event.target.svm_seqdiag.display.innerText = ''; });
			this.detectors.appendChild(d);
		}
	}
	svm_event_draw(e)
	{
		if(e==undefined) { return; }
		var svm_start = this.svm_start_space+this.svm_vertical_space*this.svm_vertical_index;
		var ctx = this.canvas.getContext("2d");
		ctx.strokeStyle=this.svm_diagram_color;
		ctx.textBaseline = "bottom";
		ctx.lineWidth = this.svm_participant_width;
		for(var pp of this.svm_participants)
		{
			var p = pp[1];
			ctx.beginPath();
			ctx.moveTo(p.svm_vertical_position,svm_start);
			ctx.lineTo(p.svm_vertical_position,svm_start+this.svm_vertical_space);
			ctx.stroke();
		}
		ctx.lineWidth = this.svm_event_width;
		ctx.beginPath();
		if(e.type=="post")
		{
			var ee = this.svm_events.get(e.id);
			this.svm_post_event_draw(ctx,ee);
			this.svm_text_event_draw(ctx,ee);
		}
		else if(e.type=="recv")
		{
			var ee = this.svm_events.get(e.ref);
			this.svm_recv_event_draw(ctx,ee);
		}
		else if(e.type=="cancel")
		{
			var ee = this.svm_events.get(e.ref);
			this.svm_cancel_event_draw(ctx,ee);
		}
		ctx.stroke();
		this.svm_markers_draw(ctx);
		this.svm_detectors_draw();
	}
	svm_diagram_draw()
	{
		var svm_position = this.svm_start_space;
		var svm_previous = 0;
		for(var p of this.svm_participants)
		{
			var pp = svm_position+Math.max(svm_previous,p[1].svm_left_event_max_size)+5;
			p[1].svm_vertical_position = pp;
			svm_previous = Math.max(p[1].svm_name_size,p[1].svm_right_event_max_size);
			svm_position = pp + this.svm_horizontal_space;
		}
		var svm_end = this.svm_start_space+this.svm_vertical_space*(1+this.svm_vertical_index);
		this.canvas.width = (svm_position+svm_previous)*Math.max(this.svm_zoom,1);
		this.canvas.height = (this.svm_start_space+svm_end+this.svm_canvas_size_advance)*Math.max(this.svm_zoom,1);
		var ctx = this.canvas.getContext("2d");
		ctx.strokeStyle=this.svm_diagram_color;
		ctx.setTransform(this.svm_zoom,0,0,this.svm_zoom,0,0);
		ctx.textBaseline = "bottom";
		ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
		ctx.lineWidth = this.svm_participant_width;
		for(var pp of this.svm_participants)
		{
			var p = pp[1];
			ctx.fillText(p.svm_name,p.svm_vertical_position-this.svm_participant_text_space,this.svm_start_space-this.svm_participant_text_space);
			ctx.beginPath();
			ctx.moveTo(p.svm_vertical_position,this.svm_start_space);
			ctx.lineTo(p.svm_vertical_position,svm_end);
			ctx.stroke();
		}
		ctx.lineWidth = this.svm_event_width;
		for(var ee of this.svm_events)
		{
			var e = ee[1];
			ctx.beginPath();
			this.svm_post_event_draw(ctx,e);
			if(e.svm_end_index!=null)
			{
				if(e.svm_cancelled)
				{
					this.svm_cancel_event_draw(ctx,e);
				}
				else
				{
					this.svm_recv_event_draw(ctx,e);
				}
			}
			ctx.stroke();
			this.svm_text_event_draw(ctx,e);
		}
		this.svm_markers_draw(ctx);
		this.svm_detectors_draw();
	}
	svm_save_markers()
	{
		return new SVM_SequenceDiagramMarkers(this.svm_participant_markers,this.svm_event_markers,this.svm_markers_color);
	}
	svm_load_markers(markers)
	{
		this.svm_event_markers = markers.events;
		this.svm_participant_markers = markers.participants;
		this.svm_markers_color = markers.color;
	}
};

