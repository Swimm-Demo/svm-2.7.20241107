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

class SVM_Interface
{
	constructor(svm_desktop,svm_main_bar,svm_application,svm_title)
	{
		this.svm_desktop = new SVM_Desktop(svm_desktop,['svm_menu','svm_windows_list','login']);
		this.svm_desktop.svm_add_bar(svm_main_bar);
		this.svm_menu = [];
		this.svm_application = svm_application;
		this.svm_title = svm_title;
		svm_controler.login = function(a,message)
		{
			a.challenge = message.challenge;
			a.login_box = a.svm_desktop.svm_add_window("login","Login","<form style=\"padding: 10px; margin-bottom: 0px;\"><label style=\"margin-left: 25px\"for=\"login\">Login</label><input style=\"margin:10px; margin-top: 20px\"id=\"login\" type=\"text\" autofocus oninput=\"if(event.target.value.length>=60) { svm_interface.svm_login_attempt(event); }\" onkeydown=\"if(event.code=='Enter') { svm_interface.svm_login_attempt(event); }\" onclick='if(event.ctrlKey) { event.target.value=\"\"; }'/><br/><div class=\"button\" style=\"text-align: center;\" onclick=\"svm_interface.svm_login_attempt(event)\">Send</button></form>",new SVM_Events(null,null,function(f){document.getElementById("login").focus();},null,function(f) {f.svm_show();},null,null,function(f) { return false; }),new SVM_PositionClever(a.svm_desktop,300,100,null),new SVM_SizeMinimum(300,100),'<div class="help_large_content">This window requires you to enter the login provided by the Simple Virtual Machine instance. Before you enter the login, you can control-click on the input to clear any text previously entered.<br/>This login is displayed by the instance in a dedicated trace:<br/><pre style="display: block;">### Simple Virtual Machine [pid] : debug ##########################\nConnection from [client IP]:[client port]. Login: <b>[generated login]</b></pre>This will ensure you are:<ul><li>either the owner of this Simple Virtual Machine instance, <b>transferring the login from the instance to this window through a secured channel</b>,</li><li>or authorized by its owner <b>who has sent you the login through a secured channel</b> to access to the application code, its related data, and interact with this Simple Virtual Machine instance.</li></ul>Any login attempt is prohibited in other cases.</div>');
		}
		svm_controler.svm_title = function(a,message)
		{
			svm_controler.svm_login = true;
			a.svm_application.innerHTML = message.title;
			a.svm_title.innerHTML += " - " + message.title;
			a.svm_desktop.svm_remove_window(a.login_box);
			a.svm_menu.push(a.svm_menu_entry("Breakpoints","svm_interface.svm_open_breakpoints_list"));
			a.svm_menu.push(a.svm_menu_entry("Machine","svm_interface.svm_open_machine"));
			a.svm_menu.push(a.svm_menu_entry("Schedulers","svm_interface.svm_open_schedulers_list"));
			a.svm_menu.push(a.svm_menu_entry("Processes","svm_interface.svm_open_processes_list"));
			a.svm_menu.push(a.svm_menu_entry("Kernels","svm_interface.svm_open_kernels_list"));
			a.svm_menu.push(a.svm_menu_entry("Events","svm_interface.svm_open_events"));
			a.svm_menu.push(a.svm_menu_entry("Plugins","svm_interface.svm_open_plugins"));
			svm_main_bar.children[0].addEventListener("click",function(event)
			{
				svm_interface.svm_open_menu();
			});
			svm_interface.svm_open_menu();
		}
	}
	svm_login_attempt(event)
	{
		var login=document.getElementById('login').value;
		svm_controler.svm_main_key = hex_sha1(login);
		var response = hex_sha1(hex_sha1(login).toLowerCase()+this.challenge).toLowerCase();
		document.getElementById('login').value = "";
		svm_controler.svm_send({ command: "login", response: response });
	}
	svm_menu_entry(i,f)
	{
		return "<div class='button' onclick='"+f+"()'>"+i+"</div>";
	}
	svm_open_menu()
	{
		this.svm_desktop.svm_add_window("svm_menu","Main menu",this.svm_menu.join('')+this.svm_menu_entry("Windows list","svm_interface.svm_open_windows_list"),null,new SVM_PositionFixed(5,5),new SVM_SizeMinimum(200,200),'<div style="width: 300px;">This menu allows you to access all elements of the Simple Virtual Machine.<br/>It also allows you to control this user interface through the <div class="button line">Windows list</div> entry.</div>');
	}
	svm_open_windows_list()
	{
		var svm_list = this.svm_desktop.svm_add_window("svm_windows_list","Windows list","<div id='svm_windows_list' style='overflow:scroll; height:600; width:500;'></div>",null,new SVM_PositionClever(this.svm_desktop,500,600,null),new SVM_SizeFixed(500,600),'<div class="help_large_content">This window is user interface specific.<br/>It displays the list of opened windows and allows you to control them from this list:<ul><li>When you click on the window title in the list, the corresponding window will get the focus,</li><li>When you click on <div class="window_title_bar" style="display:inline"><div class="window_button window_buttons_alarm" style="display:inline"></div></div>, the corresponding window will get the focus. If the button looks like <div class="window_title_bar" style="display:inline"><div class="window_button window_buttons_alarm active" style="display:inline"></div></div>, it means the window got an alert and clicking on this button will also clear the alert,</li><li>When you click on <div class="window_title_bar" style="display:inline"><div class="window_button window_buttons_hide" style="display:inline"></div></div>, the corresponding window will be hidden but still accessible from the window list by clicking on its title,</li><li>When you click on <div class="window_title_bar" style="display:inline"><div class="window_button window_buttons_close" style="display:inline"></div></div>, the corresponding window will be close and removed from the window list.</li></ul>These three buttons are also accessible on all windows within their title bar.<br/>On the title bar of windows, the button <div class="window_title_bar" style="display:inline"><div class="window_button window_buttons_help" style="display:inline"></div></div> can also be available and will open a window with contextual help.<br/>Windows can be moved either by a drag and drop from any point on its surface, or by a middle-click on its surface.<p>When a middle-click is done, a placement algorithm will try to move the window at the nearest place where it aligns with another window. If the Control key is pressed, the choosen place is also avoid overlapping another window.</p></div>');
		svm_list.svm_add_windows_list(document.getElementById('svm_windows_list'));
	}
	svm_open_machine()
	{
		var svm_view = new SVM_ViewMachine();
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_index_function(svm_view,"machine",svm_view.svm_box.svm_usable(),function(svm_view,dom,message)
			{
				var id_machine = message.identifiers[0];
				svm_controler.svm_add_object_function(svm_view,id_machine,"object",svm_view.svm_box.svm_usable(),function(svm_view,dom,message)
				{
					if((dom.children[0].children[0].innerText!="...")&&(dom.children[0].children[0].innerText!=message.state))
					{
						svm_view.svm_box.svm_notify();
					}
					dom.innerHTML = '<div>Status: <span>...</span></div><div>Version: <span>...</span></div><div class="button">Kill</div><hr/><div>Refresh view:</div><div class="button">...</div>';
					dom.children[0].children[0].innerText = message.state;
					dom.children[1].children[0].innerText = message.version;
					dom.children[2].svm_machine = id_machine;
					dom.children[2].addEventListener("click",function(event)
						{
							svm_controler.svm_send({ command: "event", identifier: event.target.svm_machine, kill:true });
						});
					dom.children[5].addEventListener("click",function(event)
						{
							svm_controler.svm_send({ command: "client", always_refresh: "toggle" });
						});
					svm_controler.svm_send({ command: "client", always_refresh: "get" });
				});
				svm_controler.svm_set_client_function(svm_view,dom,function(svm_view,dom,message)
				{
					if(message.always_refresh != null)
					{
						if(message.always_refresh)
						{
							dom.children[5].innerText = "Always";
						}
						else
						{
							dom.children[5].innerText = "On breakpoints";
						}
					}
				});
				svm_controler.svm_send({ command: "object", identifier: id_machine });
			});
		svm_controler.svm_send({ command: "index", type: "machine" });
	}
	svm_open_schedulers_list()
	{
		var svm_view = new SVM_ViewSchedulersList();
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_index_function(svm_view,"scheduler",svm_view.svm_box.svm_usable().childNodes[0],function(svm_view,dom,message)
			{
				dom.innerHTML="";
				for(var i of message.identifiers)
				{
					var ee = document.createElement('div');
					ee.classList.add("button");
					ee.innerText="...";
					ee.svm_scheduler_id=i;
					ee.addEventListener("click",function(event) { svm_interface.svm_open_scheduler(event.target.svm_scheduler_id,event.target.svm_scheduler_name); })
					dom.appendChild(ee);
					svm_controler.svm_add_object_function(svm_view,i,"object",ee,function(svm_view,dom,message)
					{
						dom.innerText = message.name;
						dom.svm_scheduler_name = message.name;
					});
				}
				svm_controler.svm_send({ command: "objects", identifiers: message.identifiers });
			});
		svm_controler.svm_send({ command: "index", type: "scheduler" });
	}
	svm_open_scheduler(id,svm_name)
	{
		var svm_view = new SVM_ViewScheduler(svm_name);
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_object_function(svm_view,id,"object",svm_view.svm_box.svm_usable().childNodes[0],function(svm_view,dom,message)
			{
				dom.innerText = message.state;
			});
		svm_controler.svm_send({ command: "object", identifier: id });
	}
	svm_open_plugins()
	{
		var svm_view = new SVM_ViewPlugins();
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_index_function(svm_view,"plugins",svm_view.svm_box.svm_usable(),function(svm_view,dom,message)
			{
				var id_extensions = message.identifiers[0];
				svm_controler.svm_add_object_function(svm_view,id_extensions,"object",svm_view.svm_box.svm_usable().childNodes[0].childNodes[0],function(svm_view,dom,message)
				{
					if(!svm_view.svm_new_view)
					{
						return;
					}
					dom.innerHTML = "<tr><th>Definitions</th></tr>";
					for(var o of message.objects)
					{
						var r = document.createElement('tr');
						var tdd = document.createElement('td');
						tdd.style = "border: solid 1px";
						tdd.innerText = o.definition;
						r.appendChild(tdd);
						dom.appendChild(r);
					}
					svm_view.svm_new_view = false;
				});
				svm_controler.svm_send({ command: "object", identifier: id_extensions });
			});
		svm_controler.svm_send({ command: "index", type: "plugins" });
	}
	svm_open_events()
	{
		var svm_view = new SVM_ViewEvents();
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_index_function(svm_view,"events",svm_view.svm_box.svm_usable(),function(svm_view,dom,message)
			{
				var id_evenements = message.identifiers[0];
				svm_controler.svm_add_object_function(svm_view,id_evenements,"event",svm_view.svm_box.svm_usable().childNodes[1],function(svm_view,dom,message)
				{
					svm_view.svm_sequence_diagram.svm_add_event(message.event);
					svm_view.svm_box.svm_notify();
				});
				svm_controler.svm_add_object_function(svm_view,id_evenements,"object",svm_view.svm_box.svm_usable().childNodes[1],function(svm_view,dom,message)
				{
					var markers = null;
					if(!svm_view.svm_new_view)
					{
						markers = svm_view.svm_sequence_diagram.svm_save_markers();
					}
					dom.innerHTML = "";
					var d = document.createElement("div");
					dom.appendChild(d);
					var c = document.createElement("canvas");
					c.style.cursor = "pointer";
					c.addEventListener("click",function(event)
						{
							svm_view.svm_sequence_diagram.svm_marker(event);
						});
					dom.appendChild(c);
					var dd = document.createElement("div");
					dom.appendChild(dd);
					svm_view.svm_sequence_diagram = new SVM_SequenceDiagram(c,dd,dom.parentNode.childNodes[0].childNodes[0]);
					for(var e of message.events)
					{
						svm_view.svm_sequence_diagram.svm_compute_event(e);
					}
					if(markers!=null)
					{
						svm_view.svm_sequence_diagram.svm_load_markers(markers);
					}
					svm_view.svm_sequence_diagram.svm_diagram_draw();
					if(svm_view.svm_new_view)
					{
						d.scrollIntoView();
					}
					svm_view.svm_new_view = false;
				});
				svm_controler.svm_send({ command: "object", identifier: id_evenements });
			});
		svm_controler.svm_send({ command: "index", type: "events" });
	}
	svm_open_processes_list()
	{
		var svm_view = new SVM_ViewProcessesList();
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_index_function(svm_view,"process",svm_view.svm_box.svm_usable().childNodes[0],function(svm_view,dom,message)
			{
				dom.innerHTML="";
				for(var i of message.identifiers)
				{
					var ee = document.createElement('div');
					ee.classList.add("button");
					ee.innerText="...";
					ee.processus_id=i;
					ee.addEventListener("click",function(event) { svm_interface.svm_open_process(event.target.processus_id,event.target.svm_process_name); })
					dom.appendChild(ee);
					svm_controler.svm_add_object_function(svm_view,i,"object",ee,function(svm_view,dom,message)
					{
						dom.innerText = message.name+' ('+message.state+')';
						dom.svm_process_name = message.name;
					});
				}
				svm_controler.svm_send({ command: "objects", identifiers: message.identifiers });
			});
		svm_controler.svm_send({ command: "index", type: "process" });
	}
	svm_open_process(id,svm_name)
	{
		var svm_view = new SVM_ViewProcess(svm_name);
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_object_function(svm_view,id,"object",svm_view.svm_box.svm_usable().childNodes[0],function(svm_view,dom,message)
			{
				dom.innerHTML = "";
				var s = document.createElement('div');
				s.innerText = "State: " + message.full_state;
				dom.appendChild(s);
				if('locking' in message)
				{
					var l = document.createElement('div');
					l.innerText = "Locking: " + message.locking.join(', ');
					dom.appendChild(l);
				}
				if('locked_by' in message)
				{
					var l = document.createElement('div');
					l.innerText = "Locked by: " + message.locked_by;
					dom.appendChild(l);
				}
				if('waiting_lock' in message)
				{
					var l = document.createElement('div');
					l.innerText = "Waiting lock: " + message.waiting_lock.join(', ');
					dom.appendChild(l);
				}
				var s = document.createElement('div');
				s.innerText = 'Sequencer';
				s.classList.add('button');
				s.addEventListener('click',function(event)
				{
					svm_interface.svm_open_sequencer("P "+svm_name.substring(8),message.sequencer,svm_view.svm_box);
				});
				dom.appendChild(s);
				if('current_kernel' in message)
				{
					var nc = document.createElement('div');
					nc.innerText = 'Current kernel ('+message.current_kernel.name+')';
					nc.classList.add('button');
					nc.addEventListener('click',function(event)
					{
						svm_interface.svm_open_kernel("P "+svm_name.substring(8),message.current_kernel.identifier,message.current_kernel.name,svm_view.svm_box);
					});
					dom.appendChild(nc);
				}
				var n = document.createElement('div');
				n.innerText = "Kernels:";
				var svm_kernels = [];
				for(var nn of message.kernels)
				{
					var sn = document.createElement('div');
					sn.innerText = '...';
					sn.classList.add('button');
					sn.svm_kernel_id=nn.identifier;
					sn.svm_kernel_name=nn.name;
					sn.addEventListener('click',function(event)
					{
						svm_interface.svm_open_kernel("P "+svm_name.substring(8),event.target.svm_kernel_id,event.target.svm_kernel_name,svm_view.svm_box);
					});
					svm_controler.svm_add_object_function(svm_view,nn.identifier,"object",sn,function(svm_view,dom,message)
					{
						dom.innerText = message.name+' ('+message.state+')';
					});
					svm_kernels.push(nn.identifier);
					n.appendChild(sn);
				}
				svm_controler.svm_send({ command: "objects", identifiers: svm_kernels });
				dom.appendChild(n);
			});
		svm_controler.svm_send({ command: "object", identifier: id });
	}
	svm_open_sequencer(svm_process,id,svm_box)
	{
		var svm_view = new SVM_ViewSequencer(svm_process,svm_box);
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_object_function(svm_view,id,"object",svm_view.svm_box.svm_usable().childNodes[0],function(svm_view,dom,message)
			{
				dom.innerText = message.state;
			});
		svm_controler.svm_send({ command: "object", identifier: id });
	}
	svm_open_kernels_list()
	{
		var svm_view = new SVM_ViewKernelsList();
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_index_function(svm_view,"kernel",svm_view.svm_box.svm_usable().childNodes[0],function(svm_view,dom,message)
			{
				dom.innerHTML="";
				for(var i of message.identifiers)
				{
					var ee = document.createElement('div');
					ee.classList.add("button");
					ee.innerText="...";
					ee.svm_kernel_id=i;
					ee.addEventListener("click",function(event) { svm_interface.svm_open_kernel("P "+event.target.svm_process_name.substring(8),event.target.svm_kernel_id,event.target.svm_kernel_name); })
					dom.appendChild(ee);
					svm_controler.svm_add_object_function(svm_view,i,"object",ee,function(svm_view,dom,message)
					{
						dom.innerText = message.process+' - '+message.name+' ('+message.state+')';
						dom.svm_kernel_name = message.name;
						dom.svm_process_name = message.process;
					});
				}
				svm_controler.svm_send({ command: "objects", identifiers: message.identifiers });
			});
		svm_controler.svm_send({ command: "index", type: "kernel" });
	}
	svm_open_kernel(svm_process,id,svm_name,svm_box)
	{
		var svm_view = new SVM_ViewKernel(svm_process,svm_name,svm_box);
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_object_function(svm_view,id,"object",svm_view.svm_box.svm_usable(),function(svm_view,dom,message)
			{
				dom.innerHTML="";
				var s = document.createElement('div');
				s.innerText = 'State: ' + message.full_state;
				dom.appendChild(s);
				if(message.state==="I")
				{
					svm_view.svm_box.svm_notify();
				}
				var p = document.createElement('div');
				p.classList.add("button");
				p.innerText = "Processor";
				p.svm_process_name = svm_process;
				p.svm_kernel_name = svm_name;
				p.svm_processor_id = message.processor;
				p.svm_box = svm_view.svm_box;
				p.addEventListener("click",function(event) { svm_interface.svm_open_processor(event.target.svm_process_name,event.target.svm_kernel_name,event.target.svm_processor_id,event.target.svm_box); })
				dom.appendChild(p);
				var m = document.createElement('div');
				m.classList.add("button");
				m.innerText = "Memory";
				m.svm_process_name = svm_process;
				m.svm_kernel_name = svm_name;
				m.svm_memory_id = message.memory;
				m.svm_box = svm_view.svm_box;
				m.addEventListener("click",function(event) { svm_interface.svm_open_memory(event.target.svm_process_name,event.target.svm_kernel_name,event.target.svm_memory_id,event.target.svm_box); })
				dom.appendChild(m);
			});
		svm_controler.svm_send({ command: "object", identifier: id });
	}
	svm_processor_instruction_overrides(iol,io,svm_view)
	{
		for(var i of io)
		{
			var svm_text = document.createTextNode(" <" + i.address.code_name + "/" + i.address.local + ">");
			iol.appendChild(svm_text);
			var svm_button = document.createElement('div');
			svm_button.classList.add("button");
			svm_button.style.marginLeft = "5px";
			svm_button.style.display = "inline-block";
			svm_button.innerText = "Code";
			svm_button.svm_code_name = "<b>Code "+i.address.code_name+"</b>"+svm_view.svm_name.substring(9);
			svm_button.svm_code_id = i.address.code;
			svm_button.svm_processor_id = svm_view.id;
			svm_button.svm_cursor = i.address.local;
			svm_button.addEventListener('click',function(event)
			{
				svm_interface.svm_open_code(false,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
			});
			iol.appendChild(svm_button);
			var svm_text2 = document.createTextNode(" => ");
			iol.appendChild(svm_text2);
			var instr = document.createElement("code");
			instr.innerText = i.instruction;
			iol.appendChild(instr);
		}
	}
	svm_processor_state(svm_state,svm_processor_id,svm_current,svm_name,svm_view)
	{
		var e = document.createElement('div');
		e.innerText = "State:";
		var pi = document.createElement('div');
		pi.innerText = "Next instruction: ";
		var vpi = document.createElement('span');
		vpi.innerText = "<" + svm_state.next_instruction.code_name + "/" + svm_state.next_instruction.local + ">";
		pi.appendChild(vpi);
		e.appendChild(pi);
		var ic = document.createElement('div');
		ic.innerText = "Current instruction: ";
		var vic = document.createElement('span');
		vic.innerText = "<" + svm_state.current_instruction.code_name + "/" + svm_state.current_instruction.local + ">";
		ic.appendChild(vic);
		var svm_button = document.createElement('div');
		svm_button.classList.add("button");
		svm_button.style.marginLeft = "5px";
		svm_button.style.display = "inline-block";
		svm_button.innerText = "Code";
		svm_button.svm_code_name = "<b>Code "+svm_state.current_instruction.code_name+"</b>"+svm_name.substring(9);
		svm_button.svm_code_id = svm_state.current_instruction.code;
		svm_button.svm_processor_id = svm_processor_id;
		svm_button.svm_cursor = svm_state.current_instruction.local;
		svm_button.addEventListener('click',function(event)
		{
			svm_interface.svm_open_code(svm_current,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
		});
		ic.appendChild(svm_button);
		if(svm_current)
		{
			var svm_explain_button = document.createElement('div');
			svm_explain_button.classList.add("button");
			svm_explain_button.style.marginLeft = "5px";
			svm_explain_button.style.display = "inline-block";
			svm_explain_button.innerText = "Explain";
			svm_explain_button.svm_processor_id = svm_processor_id;
			svm_explain_button.addEventListener('click',function(event)
			{
				svm_interface.svm_processor_explain(event.target.svm_processor_id);
			});
			ic.appendChild(svm_explain_button);
		}
		else
		{
			var svm_break_button = document.createElement('div');
			svm_break_button.classList.add("button");
			svm_break_button.classList.add("processor_return");
			svm_break_button.style.marginLeft = "5px";
			svm_break_button.style.display = "inline";
			svm_break_button.svm_processor_id = svm_processor_id;
			svm_break_button.addEventListener("click",function(event)
			{
				var svm_state = event.target.parentNode.parentNode;
				var svm_stack = svm_state.parentNode;
				for(var svm_index = 0 ; svm_stack.children[svm_index] != svm_state ; ++svm_index)
				{
				}
				var svm_level = svm_stack.childElementCount - svm_index;
				svm_controler.svm_send({command:"run", identifier: event.target.svm_processor_id, type: "return", level: svm_level});
			});
			ic.appendChild(svm_break_button);
		}
		var svm_explain = document.createElement('div');
		svm_explain.style.display = "none";
		ic.appendChild(svm_explain);
		e.appendChild(ic);
		var mc = document.createElement('div');
		mc.innerText = "Current memory: ";
		var vmc = document.createElement('span');
		vmc.innerText = "&" + svm_state.current_memory.address + "*" + svm_state.current_memory.size;
		mc.appendChild(vmc);
		e.appendChild(mc);
		var ma = document.createElement('div');
		ma.innerText = "Allocated memory:";
		var vma = document.createElement('span');
		for(var p of svm_state.allocated_memory)
		{
			vma.innerText += " &" + p.address + "*" + p.size;
		}
		ma.appendChild(vma);
		e.appendChild(ma);
		var da = document.createElement('div');
		da.innerText = "Defined aliases:";
		var vda = document.createElement('span');
		for(var a of svm_state.defined_aliases)
		{
			vda.innerText += " " + a;
		}
		da.appendChild(vda);
		e.appendChild(da);
		var li = document.createElement('div');
		li.innerText = "Local interruptions:";
		var vli = document.createElement('span');
		for(var i of svm_state.local_interruptions)
		{
			var svm_text = document.createTextNode(" " + i.interruption + " => <" + i.handler.code_name + "/" + i.handler.local + ">");
			vli.appendChild(svm_text);
			var svm_button = document.createElement('div');
			svm_button.classList.add("button");
			svm_button.style.marginLeft = "5px";
			svm_button.style.display = "inline-block";
			svm_button.innerText = "Code";
			svm_button.svm_code_name = "<b>Code "+i.handler.code_name+"</b>"+svm_name.substring(9);
			svm_button.svm_code_id = i.handler.code;
			svm_button.svm_processor_id = svm_processor_id;
			svm_button.svm_cursor = i.handler.local;
			svm_button.addEventListener('click',function(event)
			{
				svm_interface.svm_open_code(false,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
			});
			vli.appendChild(svm_button);
		}
		li.appendChild(vli);
		e.appendChild(li);
		var cli = document.createElement('div');
		cli.innerText = "Cascaded local interruptions:";
		var vcli = document.createElement('span');
		for(var i of svm_state.cascaded_local_interruptions)
		{
			var svm_text = document.createTextNode(" " + i.interruption + " => <" + i.handler.code_name + "/" + i.handler.local + ">");
			vcli.appendChild(svm_text);
			var svm_button = document.createElement('div');
			svm_button.classList.add("button");
			svm_button.style.marginLeft = "5px";
			svm_button.style.display = "inline-block";
			svm_button.innerText = "Code";
			svm_button.svm_code_name = "<b>Code "+i.handler.code_name+"</b>"+svm_name.substring(9);
			svm_button.svm_code_id = i.handler.code;
			svm_button.svm_processor_id = svm_processor_id;
			svm_button.svm_cursor = i.handler.local;
			svm_button.addEventListener('click',function(event)
			{
				svm_interface.svm_open_code(false,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
			});
			vcli.appendChild(svm_button);
		}
		cli.appendChild(vcli);
		e.appendChild(cli);
		var d = document.createElement('div');
		d.innerText = "Flags:";
		var vd = document.createElement('span');
		for(var f of svm_state.flags)
		{
			vd.innerText += " " + f;
		}
		d.appendChild(vd);
		e.appendChild(d);
		var cd = document.createElement('div');
		cd.innerText = "Cascaded flags:";
		var vcd = document.createElement('span');
		for(var f of svm_state.cascaded_flags)
		{
			vcd.innerText += " " + f;
		}
		cd.appendChild(vcd);
		e.appendChild(cd);
		var io = document.createElement('div');
		io.innerText = "Instruction overrides:";
		var iol = document.createElement('span');
		svm_interface.svm_processor_instruction_overrides(iol,svm_state.instruction_overrides,svm_view);
		io.appendChild(iol);
		e.appendChild(io);
		var ioc = document.createElement('div');
		ioc.innerText = "Cascaded instruction overrides:";
		var iolc = document.createElement('span');
		svm_interface.svm_processor_instruction_overrides(iolc,svm_state.cascaded_instruction_overrides,svm_view);
		ioc.appendChild(iolc);
		e.appendChild(ioc);
		e.style.border = "solid 1px";
		e.style.margin = "5px";
		e.style.padding = "2px";
		return e;
	}
	svm_open_processor(svm_process,svm_kernel,id,svm_box)
	{
		var svm_view = new SVM_ViewProcessor(svm_process,"K "+svm_kernel.substring(7),id,svm_box);
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_object_function(svm_view,id,"event",svm_view.svm_box.svm_usable().children[0],function(svm_view,dom,message)
			{
				if(message.event.state != null)
				{
					dom.children[1].replaceChild(svm_interface.svm_processor_state(message.event.state,svm_view.id,true,svm_view.svm_name,svm_view),dom.children[1].children[0]);
				}
				if(message.event.current_instruction != null)
				{
					dom.children[1].children[0].children[1].children[0].innerText = "<" + message.event.current_instruction.code_name + "/" + message.event.current_instruction.local + ">";
					dom.children[1].children[0].children[1].children[1].svm_code_name = "<b>Code " + message.event.current_instruction.code_name+"</b> "+svm_view.svm_name.substring(9);
					dom.children[1].children[0].children[1].children[1].svm_code_id = message.event.current_instruction.code;
					dom.children[1].children[0].children[1].children[1].svm_cursor = message.event.current_instruction.local;
					dom.children[1].children[0].children[1].children[3].style.display = "none";
					dom.children[1].children[0].children[1].children[3].innerHTML = "";
				}
				if(message.event.next_instruction != null)
				{
					dom.children[1].children[0].children[0].children[0].innerText = "<" + message.event.next_instruction.code_name + "/" + message.event.next_instruction.local + ">";
				}
				if(message.event.current_memory != null)
				{
					dom.children[1].children[0].children[2].children[0].innerText = "&" + message.event.current_memory.address + "*" + message.event.current_memory.size;
				}
				if(message.event.allocated_memory != null)
				{
					dom.children[1].children[0].children[3].children[0].innerText = "";
					for(var p of message.event.allocated_memory)
					{
						dom.children[1].children[0].children[3].children[0].innerText += " &" + p.address + "*" + p.size;
					}
				}
				if(message.event.defined_aliases != null)
				{
					dom.children[1].children[0].children[4].children[0].innerText = "";
					for(var a of message.event.defined_aliases)
					{
						dom.children[1].children[0].children[4].children[0].innerText += " " + a;
					}
				}
				if(message.event.local_interruptions != null)
				{
					dom.children[1].children[0].children[5].children[0].innerText = "";
					for(var i of message.event.local_interruptions)
					{
						var svm_text = document.createTextNode(" " + i.interruption + " => <" + i.handler.code_name + "/" + i.handler.local + ">");
						dom.children[1].children[0].children[5].children[0].appendChild(svm_text);
						var svm_button = document.createElement('div');
						svm_button.classList.add("button");
						svm_button.style.marginLeft = "5px";
						svm_button.style.display = "inline-block";
						svm_button.innerText = "Code";
						svm_button.svm_code_name = "<b>Code "+i.handler.code_name+"</b>"+svm_view.svm_name.substring(9);
						svm_button.svm_code_id = i.handler.code;
						svm_button.svm_processor_id = svm_view.id;
						svm_button.svm_cursor = i.handler.local;
						svm_button.addEventListener('click',function(event)
							{
								svm_interface.svm_open_code(false,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
							});
						dom.children[1].children[0].children[5].children[0].appendChild(svm_button);
					}
				}
				if(message.event.cascaded_local_interruptions != null)
				{
					dom.children[1].children[0].children[6].children[0].innerText = "";
					for(var i of message.event.cascaded_local_interruptions)
					{
						var svm_text = document.createTextNode(" " + i.interruption + " => <" + i.handler.code_name + "/" + i.handler.local + ">");
						dom.children[1].children[0].children[6].children[0].appendChild(svm_text);
						var svm_button = document.createElement('div');
						svm_button.classList.add("button");
						svm_button.style.marginLeft = "5px";
						svm_button.style.display = "inline-block";
						svm_button.innerText = "Code";
						svm_button.svm_code_name = "<b>Code "+i.handler.code_name+"</b>"+svm_view.svm_name.substring(9);
						svm_button.svm_code_id = i.handler.code;
						svm_button.svm_processor_id = svm_view.id;
						svm_button.svm_cursor = i.handler.local;
						svm_button.addEventListener('click',function(event)
							{
								svm_interface.svm_open_code(false,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
							});
						dom.children[1].children[0].children[6].children[0].appendChild(svm_button);
					}
				}
				if(message.event.global_interruptions != null)
				{
					dom.children[1].children[2].children[0].innerText = "";
					for(var i of message.event.global_interruptions)
					{
						var svm_text = document.createTextNode(" " + i.interruption + " => <" + i.handler.code_name + "/" + i.handler.local + ">");
						dom.children[1].children[2].children[0].appendChild(svm_text);
						var svm_button = document.createElement('div');
						svm_button.classList.add("button");
						svm_button.style.marginLeft = "5px";
						svm_button.style.display = "inline-block";
						svm_button.innerText = "Code";
						svm_button.svm_code_name = "<b>Code "+i.handler.code_name+"</b>"+svm_view.svm_name.substring(9);
						svm_button.svm_code_id = i.handler.code;
						svm_button.svm_processor_id = svm_view.id;
						svm_button.svm_cursor = i.handler.local;
						svm_button.addEventListener('click',function(event)
							{
								svm_interface.svm_open_code(false,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
							});
						dom.children[1].children[2].children[0].appendChild(svm_button);
					}
				}
				if(message.event.flags != null)
				{
					dom.children[1].children[0].children[7].children[0].innerText = "";
					for(var f of message.event.flags)
					{
						dom.children[1].children[0].children[7].children[0].innerText += " " + f;
					}
				}
				if(message.event.cascaded_flags != null)
				{
					dom.children[1].children[0].children[8].children[0].innerText = "";
					for(var f of message.event.cascaded_flags)
					{
						dom.children[1].children[0].children[8].children[0].innerText += " " + f;
					}
				}
				if(message.event.waiting_interruptions != null)
				{
					dom.children[1].children[3].children[1].innerText = "";
					for(var i of message.event.waiting_interruptions)
					{
						dom.children[1].children[3].children[1].innerText += " " + i;
					}
				}
				if(message.event.interruptions_enabled != null)
				{
					if(message.event.interruptions_enabled)
					{
						dom.children[1].children[3].children[0].innerText = "";
					}
					else
					{
						dom.children[1].children[3].children[0].innerText = " (on hold)";
					}
				}
				if(message.event.instruction_overrides != null)
				{
					dom.children[1].children[0].children[9].children[0].innerText = "";
					svm_interface.svm_processor_instruction_overrides(dom.children[1].children[0].children[9].children[0],message.event.instruction_overrides,svm_view);
				}
				if(message.event.cascaded_instruction_overrides != null)
				{
					dom.children[1].children[0].children[10].children[0].innerText = "";
					svm_interface.svm_processor_instruction_overrides(dom.children[1].children[0].children[10].children[0],message.event.cascaded_instruction_overrides,svm_view);
				}
				if(message.event.global_instruction_overrides != null)
				{
					dom.children[1].children[4].children[0].innerText = "";
					svm_interface.svm_processor_instruction_overrides(dom.children[1].children[4].children[0],message.event.global_instruction_overrides,svm_view);
				}
				if(message.event.push_state != null)
				{
					dom.children[1].children[1].children[0].insertBefore(svm_interface.svm_processor_state(message.event.push_state,svm_view.id,false,svm_view.svm_name,svm_view),dom.children[1].children[1].children[0].firstChild);
				}
				if(message.event.pop_state != null)
				{
					dom.children[1].children[1].children[0].removeChild(dom.children[1].children[1].children[0].firstChild);
					dom.children[1].replaceChild(svm_interface.svm_processor_state(message.event.pop_state,svm_view.id,true,svm_view.svm_name,svm_view),dom.children[1].children[0]);
				}
				if(message.event.quota != null)
				{
					if(message.event.quota.current != null)
					{
						dom.children[1].children[5].children[0].innerText = message.event.quota.current;
					}
					dom.children[1].children[5].children[1].innerText = message.event.quota.limit;
				}
				if(message.event.add_breakpoint)
				{
					svm_view.svm_breakpoints.push(message.event.add_breakpoint);
				}
				if(message.event.remove_breakpoint)
				{
					svm_view.svm_breakpoints = svm_view.svm_breakpoints.filter(function(svm_address) { return !((svm_address.code == message.event.remove_breakpoint.code) && (svm_address.local == message.event.remove_breakpoint.local)); });
				}
				if(message.event.explain)
				{
					dom.children[1].children[0].children[1].children[3].innerHTML = "";
					var instruction_text_td = document.createElement("td");
					instruction_text_td.classList.add("code");
					instruction_text_td.innerHTML = message.event.explain.text;
					var instruction_text_tr = document.createElement("tr");
					instruction_text_tr.classList.add("code");
					instruction_text_tr.appendChild(instruction_text_td);
					var instruction_text_table = document.createElement("table");
					instruction_text_table.appendChild(instruction_text_tr);
					dom.children[1].children[0].children[1].children[3].appendChild(instruction_text_table);
					var instruction_parameters_table = document.createElement("table");
					for(var e of message.event.explain.parameters)
					{
						var instruction_parameters_td = document.createElement("td");
						instruction_parameters_td.style.border = "solid 1px";
						var instruction_parameters_pre = document.createElement("pre");
						instruction_parameters_pre.innerHTML = e;
						instruction_parameters_td.appendChild(instruction_parameters_pre);
						var instruction_parameters_tr = document.createElement("tr");
						instruction_parameters_tr.appendChild(instruction_parameters_td);
						instruction_parameters_table.appendChild(instruction_parameters_tr);
					}
					dom.children[1].children[0].children[1].children[3].appendChild(instruction_parameters_table);
					dom.children[1].children[0].children[1].children[3].style.display = "block";
					dom.children[1].children[0].children[1].children[3].style.overflow = "auto";
					dom.children[1].children[0].children[1].children[3].style.resize = "vertical";
					dom.children[1].children[0].children[1].children[3].style.border = "solid 1px";
					dom.children[1].children[0].children[1].children[3].style.marginLeft = "20px";
					dom.children[1].children[0].children[1].children[3].style.marginRight = "20px";
				}
			});
		svm_controler.svm_add_object_function(svm_view,id,"object",svm_view.svm_box.svm_usable().children[0],function(svm_view,dom,message)
			{
				dom.innerHTML="";
				var svm_buttons = document.createElement("div");
				var svm_pause = document.createElement("div");
				svm_pause.classList.add("button");
				svm_pause.classList.add("processor_pause");
				svm_pause.style.display = "inline-block";
				svm_pause.style.margin = "2px";
				svm_pause.style.verticalAlign = "middle";
				svm_pause.svm_processor_id = id;
				svm_pause.addEventListener("click",function(event)
				{
					svm_controler.svm_send({command:"event", identifier: event.target.svm_processor_id, pause: true});
				});
				svm_buttons.appendChild(svm_pause);
				svm_buttons.style.borderBottom="solid 1px";
				svm_buttons.style.height = "30px";
				var svm_instruction = document.createElement("div");
				svm_instruction.classList.add("button");
				svm_instruction.classList.add("processor_instruction");
				svm_instruction.style.display = "inline-block";
				svm_instruction.style.margin = "2px";
				svm_instruction.style.verticalAlign = "middle";
				svm_instruction.svm_processor_id = id;
				svm_instruction.addEventListener("click",function(event)
				{
					svm_controler.svm_send({command:"run", identifier: event.target.svm_processor_id, type: "instruction"});
				});
				svm_buttons.appendChild(svm_instruction);
				var svm_function = document.createElement("div");
				svm_function.classList.add("button");
				svm_function.classList.add("processor_function");
				svm_function.style.display = "inline-block";
				svm_function.style.margin = "2px";
				svm_function.style.verticalAlign = "middle";
				svm_function.svm_processor_id = id;
				svm_function.addEventListener("click",function(event)
				{
					svm_controler.svm_send({command:"run", identifier: event.target.svm_processor_id, type: "function"});
				});
				svm_buttons.appendChild(svm_function);
				var svm_run = document.createElement("div");
				svm_run.classList.add("button");
				svm_run.classList.add("processor_run");
				svm_run.style.display = "inline-block";
				svm_run.style.margin = "2px";
				svm_run.style.verticalAlign = "middle";
				svm_run.svm_processor_id = id;
				svm_run.addEventListener("click",function(event)
				{
					svm_controler.svm_send({command:"run", identifier: event.target.svm_processor_id});
				});
				svm_buttons.appendChild(svm_run);
				dom.appendChild(svm_buttons);
				var rest = document.createElement("div");
				rest.style.overflow="scroll";
				rest.style.height=svm_view.svm_box.svm_size.y-svm_buttons.clientHeight;
				var e = svm_interface.svm_processor_state(message.state,svm_view.id,true,svm_view.svm_name,svm_view);
				rest.appendChild(e);
				var rs = document.createElement("div");
				rs.style.border = "solid 1px";
				rs.style.margin = "5px";
				rs.style.padding = "2px";
				rs.innerText = "Return stack:";
				var rss = document.createElement("div");
				rs.appendChild(rss);
				for(var ss of message.return_stack)
				{
					var ee = svm_interface.svm_processor_state(ss,svm_view.id,false,svm_view.svm_name,svm_view);
					ee.style.border = "solid 1px";
					ee.style.margin = "5px";
					ee.style.padding = "2px";
					rss.appendChild(ee);
				}
				rest.appendChild(rs);
				var gi = document.createElement("div");
				gi.style.border = "solid 1px";
				gi.style.margin = "5px";
				gi.style.padding = "2px";
				gi.innerText = "Global interruptions:";
				var vgi = document.createElement('span');
				for(var i of message.global_interruptions)
				{
					var svm_text = document.createTextNode(" " + i.interruption + " => <" + i.handler.code_name + "/" + i.handler.local + ">");
					vgi.appendChild(svm_text);
					var svm_button = document.createElement('div');
					svm_button.classList.add("button");
					svm_button.style.marginLeft = "5px";
					svm_button.style.display = "inline-block";
					svm_button.innerText = "Code";
					svm_button.svm_code_name = "<b>Code "+i.handler.code_name+"</b>"+svm_view.svm_name.substring(9);
					svm_button.svm_code_id = i.handler.code;
					svm_button.svm_processor_id = id;
					svm_button.svm_cursor = i.handler.local;
					svm_button.addEventListener('click',function(event)
					{
						svm_interface.svm_open_code(false,event.target.svm_code_name,event.target.svm_code_id,event.target.svm_processor_id,event.target.svm_cursor,svm_view.svm_breakpoints,svm_view.svm_box);
					});
					vgi.appendChild(svm_button);
				}
				gi.appendChild(vgi);
				rest.appendChild(gi);
				var wi = document.createElement("div");
				wi.style.border = "solid 1px";
				wi.style.margin = "5px";
				wi.style.padding = "2px";
				wi.innerText = "Waiting interruptions";
				var vws = document.createElement('span');
				if(!message.interruptions_enabled)
				{
					vws.innerText = " (on hold)";
				}
				wi.appendChild(vws);
				var vwit = document.createTextNode(":");
				wi.appendChild(vwit);
				var vwi = document.createElement('span');
				for(var i of message.waiting_interruptions)
				{
					vwi.innerText += " " + i;
				}
				wi.appendChild(vwi);
				rest.appendChild(wi);
				var io = document.createElement("div");
				io.style.border = "solid 1px";
				io.style.margin = "5px";
				io.style.padding = "2px";
				io.innerText = "Global instruction overrides:";
				var iol = document.createElement('span');
				svm_interface.svm_processor_instruction_overrides(iol,message.global_instruction_overrides,svm_view);
				io.appendChild(iol);
				rest.appendChild(io);
				if(message.quota != null)
				{
					var q = document.createElement("div");
					q.style.border = "solid 1px";
					q.style.margin = "5px";
					q.style.padding = "2px";
					q.innerText = "Quota: ";
					if(message.quota.current != null)
					{
						q.innerText += " current=";
						var qc = document.createElement("span");
						qc.innerText = message.quota.current;
						q.appendChild(qc);
					}
					q.innerHTML += " limit=";
					var ql = document.createElement("span");
					ql.innerText = message.quota.limit;
					q.appendChild(ql);
					rest.appendChild(q);
				}
				dom.appendChild(rest);
				svm_view.svm_breakpoints = message.breakpoints;
			});
		svm_controler.svm_send({ command: "object", identifier: id });
	}
	svm_processor_explain(processor)
	{
		svm_controler.svm_send({ command: "event", identifier: processor, explain: true });
	}
	svm_code_cursor_scroll(dom,force)
	{
		var svm_view = dom.parentNode.parentNode.svm_window.svm_view;
		if(dom.children[2].children[0].checked||(force!=null))
		{
			var index = svm_view.svm_cursor - parseInt(dom.children[2].children[2].value);
			if(index<0) { index = 0; }
			dom.children[1].children[0].children[index].scrollIntoView({ behavior: "smooth"});
		}
	}
	svm_display_code(dom,svm_address)
	{
		dom.children[1].children[0].children[parseInt(svm_address)].scrollIntoView({ behavior: "smooth"});
	}
	svm_open_code(svm_current_state,svm_name,svm_code_id,svm_processor_id,svm_cursor,svm_breakpoints,svm_box)
	{
		var svm_view = new SVM_ViewCode((svm_current_state?"":"=")+svm_name,svm_code_id,svm_processor_id,svm_current_state,svm_cursor,svm_breakpoints,svm_box);
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_object_function(svm_view,svm_code_id,"object",svm_view.svm_box.svm_usable(),function(svm_view,dom,message)
		{
			dom.children[1].innerHTML = "";
			var ti = document.createElement("table");
			ti.classList.add("code");
			var svm_address = 0;
			for(var i of message.instructions)
			{
				var ri = document.createElement("tr");
				ri.classList.add("code");
				var pa = document.createElement("td");
				pa.classList.add("break");
				pa.svm_code_id = svm_code_id;
				pa.svm_code_address = svm_address;
				pa.svm_processor_id = svm_processor_id;
				if(svm_view.svm_breakpoints.findIndex(function(p) { return (p.code == svm_code_id) && (p.local==svm_address); })>=0)
				{
					pa.classList.add("active");
				}
				pa.addEventListener("click",function(event)
				{
					if(event.target.classList.contains("active"))
					{
						svm_controler.svm_send({command:"event", identifier: event.target.svm_processor_id, remove_breakpoint: { code: event.target.svm_code_id, local: event.target.svm_code_address}});
					}
					else
					{

						svm_controler.svm_send({command:"event", identifier: event.target.svm_processor_id, add_breakpoint: { code: event.target.svm_code_id, local: event.target.svm_code_address}});
					}
				});
				ri.appendChild(pa);
				var ip = document.createElement("td");
				ip.classList.add("cursor");
				ri.appendChild(ip);
				var t = document.createElement("td");
				t.classList.add("code");
				if(i.labels != null)
				{
					t.innerHTML += i.labels;
					var b = document.createElement("br");
					t.appendChild(b);
				}
				if(i.text != null)
				{
					t.innerHTML += i.text;
				}
				if(i.text != null)
				{
					var svm_indent = document.createElement("span");
					svm_indent.classList.add("indent");
					t.lastChild.insertBefore(svm_indent,t.lastChild.firstChild);
				}
				t.svm_instruction_extra = i.extra;
				t.addEventListener("mouseenter", function(event)
				{
					if(event.target.svm_instruction_extra != null)
					{
						event.target.parentNode.parentNode.parentNode.parentNode.firstChild.innerHTML = event.target.svm_instruction_extra;
					}
				});
				t.addEventListener("mouseleave", function(event)
				{
					event.target.parentNode.parentNode.parentNode.parentNode.firstChild.innerHTML = "";
				});
				ri.appendChild(t);
				ti.appendChild(ri);
				svm_address++;
			}
			dom.children[1].appendChild(ti);
			if(svm_view.svm_new_view && (ti.clientHeight>dom.children[1].clientHeight))
			{
				dom.children[2].children[0].checked=true;
			}
			if(svm_view.svm_cursor != null)
			{
				if(svm_view.svm_current)
				{
					ti.children[svm_view.svm_cursor].children[1].classList.add("active");
				}
				else
				{
					ti.children[svm_view.svm_cursor].children[1].classList.add("index");
				}
				if(svm_view.svm_new_view)
				{
					svm_interface.svm_code_cursor_scroll(dom);
				}
			}
			if(svm_view.svm_new_view)
			{
				for(var sym of message.symbols)
				{
					var option = document.createElement('option');
					option.value = sym.address;
					option.innerText = ':symbol ' + sym.name;
					dom.children[3].children[0].appendChild(option);
				}
				for(var etq of message.labels)
				{
					var option = document.createElement('option');
					option.value = etq.address;
					option.innerText = ':label ' + etq.name;
					dom.children[3].children[0].appendChild(option);
				}
			}
			if(svm_view.svm_new_view && svm_current_state)
			{
				var svm_size = ti.clientHeight+20;
				if(svm_size<200) { svm_size=200; }
				if(svm_size>600) { svm_size=600; }
				dom.children[1].style.height = svm_view.svm_size = svm_size;
			}
			svm_view.svm_new_view=false;
		});
		svm_controler.svm_add_object_function(svm_view,svm_processor_id,"object",svm_view.svm_box.svm_usable().children[1],function(svm_view,dom,message)
		{
			if(svm_view.svm_current)
			{
				if(message.state.current_instruction != null)
				{
					if(message.state.current_instruction.code == svm_view.code)
					{
						if(svm_view.svm_cursor != null)
						{
							dom.children[0].children[svm_view.svm_cursor].children[1].classList.remove("active");
						}
						svm_view.svm_cursor = message.state.current_instruction.local;
						if(dom.children[0] != null)
						{
							dom.children[0].children[svm_view.svm_cursor].children[1].classList.add("active");
						}
						svm_interface.svm_code_cursor_scroll(dom.parentNode);
					}
				}
			}
		});
		svm_controler.svm_add_object_function(svm_view,svm_processor_id,"event",svm_view.svm_box.svm_usable().children[1],function(svm_view,dom,message)
		{
			if(svm_view.svm_current)
			{
				if(message.event.current_instruction != null)
				{
					if(svm_view.svm_cursor != null)
					{
						if(dom.children[0] != null)
						{
							dom.children[0].children[svm_view.svm_cursor].children[1].classList.remove("active");
						}
						svm_view.svm_cursor = null;
					}
					if(message.event.current_instruction.code == svm_view.code)
					{
						svm_view.svm_cursor = message.event.current_instruction.local;
						if(dom.children[0] != null)
						{
							dom.children[0].children[svm_view.svm_cursor].children[1].classList.add("active");
						}
					}
					svm_view.svm_box.svm_acknowledge();
				}
			}
			if(message.event.add_breakpoint != null)
			{
				if(message.event.add_breakpoint.code == svm_view.code)
				{
					svm_view.svm_breakpoints.push(message.event.add_breakpoint);
					dom.children[0].children[message.event.add_breakpoint.local].children[0].classList.add("active");
				}
			}
			if(message.event.remove_breakpoint != null)
			{
				if(message.event.remove_breakpoint.code == svm_view.code)
				{
					svm_view.svm_breakpoints = svm_view.svm_breakpoints.filter(function(svm_address) { return !((svm_address.code == message.event.remove_breakpoint.code) && (svm_address.local == message.event.remove_breakpoint.local)); });
					dom.children[0].children[message.event.remove_breakpoint.local].children[0].classList.remove("active");
				}
			}
			if((svm_view.svm_current) && (message.event.breakpoint != null))
			{
				svm_view.svm_box.svm_notify();
				svm_interface.svm_code_cursor_scroll(dom.parentNode);
			}
		});
		svm_controler.svm_send({ command: "object", identifier: svm_code_id });
	}
	svm_open_memory(svm_process,svm_kernel,id,svm_box)
	{
		var svm_view = new SVM_ViewMemory(svm_process,"K "+svm_kernel.substring(7),id,svm_box);
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_object_function(svm_view,id,"event",svm_view.svm_box.svm_usable().children[0],function(svm_view,dom,message)
			{
				if((dom.children[0] == null)||(dom.children[1] == null))
				{
					return;
				}
				if(message.event.current != null)
				{
					svm_interface.svm_add_memory_highlight(svm_view,dom.children[0],message.event.current);
				}
				if(message.event.content != null)
				{
					for(var e of message.event.content)
					{
						while(dom.children[0].children.length <= (e.address+1))
						{
							var ee = document.createElement('tr');
							var ea = document.createElement('td');
							ea.style = 'border: solid 1px';
							ee.appendChild(ea);
							var et = document.createElement('td');
							et.style = 'border: solid 1px';
							ee.appendChild(et);
							var ev = document.createElement('td');
							ev.classList.add("button");
							ev.style = 'border: solid 1px';
							ev.svm_memory_id = id;
							ev.svm_memory_address = dom.children[0].childElementCount-1;
							ev.addEventListener("click",function(event)
							{
								svm_controler.svm_send({ command: "event", identifier: event.target.svm_memory_id, "sync": event.target.svm_memory_address });
							});
							ee.appendChild(ev);
							dom.children[0].appendChild(ee);
						}
						dom.parentNode.children[1].children[0].svm_maximum = dom.children[0].children.length-2;
						if(e.type != null)
						{
							dom.children[0].children[e.address+1].children[0].innerText = "&" + e.address;
							dom.children[0].children[e.address+1].children[1].innerText = e.type;
							if(e.value != null)
							{
								dom.children[0].children[e.address+1].children[2].innerText = e.value;
								if(e.pointer != null)
								{
									var pb = document.createElement('div');
									pb.classList.add("button");
									pb.classList.add("line");
									pb.classList.add("follow");
									pb.svm_pointer = e.pointer;
									pb.svm_view = svm_view;
									pb.addEventListener('click',function(event)
									{
										svm_interface.svm_add_memory_highlight(event.target.svm_view,event.target.parentNode.parentNode.parentNode,event.target.svm_pointer);
									});
									dom.children[0].children[e.address+1].children[2].appendChild(pb);
								}
							}
							else
							{
								dom.children[0].children[e.address+1].children[2].innerText = "";
							}
						}
						else
						{
							dom.children[0].children[e.address+1].children[0].innerText = "";
							dom.children[0].children[e.address+1].children[1].innerText = "";
							dom.children[0].children[e.address+1].children[2].innerText = "";
						}
					}
				}
				if((message.event.alias != null)&&(message.event.alias.length>0))
				{
					for(var a of message.event.alias)
					{
						var it = 1;
						while((it<dom.children[1].children.length)&&(dom.children[1].children[it].children[0].innerText != a.alias))
						{
							++it;
						}
						if(it==dom.children[1].children.length)
						{
							if(a.pointer == null)
							{
								continue;
							}
							var aaa = document.createElement('tr');
							var aaaa = document.createElement('td');
							aaaa.style = 'border: solid 1px';
							aaaa.innerText = a.alias;
							aaa.appendChild(aaaa);
							var aaap = document.createElement('td');
							aaap.style = 'border: solid 1px';
							aaap.innerText = '&'+a.pointer.address+'*'+a.pointer.size;
							var pb = document.createElement('div');
							pb.classList.add("button");
							pb.classList.add("line");
							pb.classList.add("follow");
							pb.svm_pointer = a.pointer;
							pb.svm_view = svm_view;
							pb.addEventListener('click',function(event)
								{
									svm_interface.svm_add_memory_highlight(event.target.svm_view,event.target.parentNode.parentNode.parentNode.parentNode.children[0],event.target.svm_pointer);
								});
							aaap.appendChild(pb);
							aaa.appendChild(aaap);
							dom.children[1].appendChild(aaa);
							continue;
						}
						if(a.pointer != null)
						{
							dom.children[1].children[it].children[1].innerHTML = "";
							dom.children[1].children[it].children[1].innerText = '&'+a.pointer.address+'*'+a.pointer.size;
							var pb = document.createElement('div');
							pb.classList.add("button");
							pb.classList.add("line");
							pb.classList.add("follow");
							pb.svm_pointer = a.pointer;
							pb.svm_view = svm_view;
							pb.addEventListener('click',function(event)
								{
									svm_interface.svm_add_memory_highlight(event.target.svm_view,event.target.parentNode.parentNode.parentNode.parentNode.children[0],event.target.svm_pointer);
								});
							dom.children[1].children[it].children[1].appendChild(pb);
							continue;
						}
						dom.children[1].removeChild(dom.children[1].children[it]);
					}
				}
				if(message.event.quota != null)
				{
					dom.children[2].children[0].innerText = message.event.quota.current;
					dom.children[2].children[1].innerText = message.event.quota.limit;
				}
			});
		svm_controler.svm_add_object_function(svm_view,id,"object",svm_view.svm_box.svm_usable().children[0],function(svm_view,dom,message)
			{
				dom.innerHTML="";
				var m = document.createElement('table');
				m.style='border: solid 1px';
				var t = document.createElement('tr');
				m.appendChild(t);
				var ta = document.createElement('th');
				ta.innerText = "Address";
				ta.style='border: solid 1px';
				t.appendChild(ta);
				var tt = document.createElement('th');
				tt.innerText = "Type";
				tt.style='border: solid 1px';
				t.appendChild(tt);
				var tv = document.createElement('th');
				tv.innerText = "Value";
				tv.style='border: solid 1px';
				t.appendChild(tv);
				var svm_last = 0;
				if(message.content.length>0)
				{
					svm_last = message.content[message.content.length-1].address+1;
				}
				for(var i=0 ; i<svm_last ; i++)
				{
					var ee = document.createElement('tr');
					var eea = document.createElement('td');
					eea.style='border: solid 1px';
					ee.appendChild(eea);
					var eet = document.createElement('td');
					eet.style='border: solid 1px';
					ee.appendChild(eet);
					var eev = document.createElement('td');
					eev.classList.add("button");
					eev.style='border: solid 1px';
					eev.svm_memory_id = id;
					eev.svm_memory_address = m.childElementCount-1;
					eev.addEventListener("click",function(event)
					{
						svm_controler.svm_send({ command: "event", identifier: event.target.svm_memory_id, "sync": event.target.svm_memory_address });
					});
					ee.appendChild(eev);
					m.appendChild(ee);
				}
				dom.parentNode.children[1].children[0].svm_maximum = svm_last-1;
				for(var e of message.content)
				{
					m.children[e.address+1].children[0].memory_address = e.address;
					m.children[e.address+1].children[0].innerText = '&'+e.address;
					m.children[e.address+1].children[1].innerText = e.type;
					if(e.value != null)
					{
						m.children[e.address+1].children[2].innerText = e.value;
						if(e.pointer != null)
						{
							var pb = document.createElement('div');
							pb.classList.add("button");
							pb.classList.add("line");
							pb.classList.add("follow");
							pb.svm_pointer = e.pointer;
							pb.svm_view = svm_view;
							pb.addEventListener('click',function(event)
							{
								svm_interface.svm_add_memory_highlight(event.target.svm_view,event.target.parentNode.parentNode.parentNode,event.target.svm_pointer);
							});
							m.children[e.address+1].children[2].appendChild(pb);
						}
					}
				}
				dom.appendChild(m);
				if(svm_view.svm_new_view)
				{
					m.scrollIntoView();
					svm_view.svm_new_view = false;
				}
				var a = document.createElement('table');
				a.style = 'border: solid 1px';
				var at = document.createElement('tr');
				a.appendChild(at);
				var ata = document.createElement('th');
				ata.innerText = "Alias";
				ata.style='border: solid 1px';
				at.appendChild(ata);
				var atp = document.createElement('th');
				atp.innerText = "Pointer";
				atp.style='border: solid 1px';
				at.appendChild(atp);
				for(var aa of message.alias)
				{
					var aaa = document.createElement('tr');
					var aaaa = document.createElement('td');
					aaaa.style = 'border: solid 1px';
					aaaa.innerText = aa.alias;
					aaa.appendChild(aaaa);
					var aaap = document.createElement('td');
					aaap.style = 'border: solid 1px';
					aaap.innerText = '&'+aa.pointer.address+'*'+aa.pointer.size;
					var pb = document.createElement('div');
					pb.classList.add("button");
					pb.classList.add("line");
					pb.classList.add("follow");
					pb.svm_pointer = aa.pointer;
					pb.svm_view = svm_view;
					pb.addEventListener('click',function(event)
						{
							svm_interface.svm_add_memory_highlight(event.target.svm_view,event.target.parentNode.parentNode.parentNode.parentNode.children[0],event.target.svm_pointer);
						});
					aaap.appendChild(pb);
					aaa.appendChild(aaap);
					a.appendChild(aaa);
				}
				dom.appendChild(a);
				if(message.quota != null)
				{
					var q = document.createElement("div");
					q.style.border = "solid 1px";
					q.style.padding = "2px";
					q.innerText = "Quota: ";
					if(message.quota.current != null)
					{
						q.innerText += " current=";
						var qc = document.createElement("span");
						qc.innerText = message.quota.current;
						q.appendChild(qc);
					}
					q.innerHTML += " limit=";
					var ql = document.createElement("span");
					ql.innerText = message.quota.limit;
					q.appendChild(ql);
					dom.appendChild(q);
				}
			});
		svm_controler.svm_send({ command: "object", identifier: id });
	}
	svm_formular(form)
	{
		var svm_formular = document.createElement("div");
		svm_formular.classList.add("form");
		var svm_title = document.createElement("div");
		svm_title.innerText = form.title + ":";
		svm_formular.appendChild(svm_title);
		var svm_fields = document.createElement("div");
		for(var c of form.fields)
		{
			var svm_field = document.createElement("div");
			svm_field.classList.add("field");
			if(c.type == "checkbox")
			{
				if(form.submitted != null)
				{
					var svm_checkbox = document.createElement("pre");
					svm_checkbox.style.display = "inline";
					if(!c.value)
					{
						svm_checkbox.innerText = "no ";
					}
				}
				else
				{
					var svm_checkbox = document.createElement("input");
					svm_checkbox.type = "checkbox";
					svm_checkbox.checked = c.value;
				}
				svm_field.appendChild(svm_checkbox);
				var svm_label = document.createElement("span");
				svm_label.classList.add("explanation");
				svm_label.innerText = c.label;
				svm_field.appendChild(svm_label);
				var svm_validity = document.createElement("span");
				svm_field.appendChild(svm_validity);
			}
			else if(c.type == "selection")
			{
				var svm_label = document.createElement("span");
				svm_label.classList.add("label");
				svm_label.innerText = c.label;
				svm_field.appendChild(svm_label);
				if(form.submitted != null)
				{
					var selection = document.createElement("pre");
					selection.style.display = "inline";
					selection.innerText = c.value;
				}
				else
				{
					var selection = document.createElement("select");
					for(var v of c.values)
					{
						var item = document.createElement("option");
						item.value = v;
						item.innerText = v;
						selection.appendChild(item);
					}
				}
				svm_field.appendChild(selection);
				var svm_validity = document.createElement("span");
				svm_field.appendChild(svm_validity);
			}
			else if(c.type == "integer")
			{
				var svm_label = document.createElement("span");
				svm_label.classList.add("label");
				svm_label.innerText = c.label;
				svm_field.appendChild(svm_label);
				if(form.submitted != null)
				{
					var svm_integer = document.createElement("pre");
					svm_integer.style.display = "inline";
					svm_integer.innerText = c.value;
				}
				else
				{
					var svm_integer = document.createElement("input");
					svm_integer.type = "number";
					if(c.svm_minimum!=null)
					{
						svm_integer.svm_minimum = c.svm_minimum;
					}
					if(c.svm_maximum!=null)
					{
						svm_integer.svm_maximum = c.svm_maximum;
					}
					svm_integer.value = c.value;
				}
				svm_field.appendChild(svm_integer);
				var svm_validity = document.createElement("span");
				svm_field.appendChild(svm_validity);
			}
			else if(c.type == "string")
			{
				var svm_label = document.createElement("span");
				svm_label.classList.add("label");
				svm_label.innerText = c.label;
				svm_field.appendChild(svm_label);
				if(form.submitted != null)
				{
					var svm_string = document.createElement("pre");
					svm_string.style.display = "inline";
					svm_string.innerText = c.value;
				}
				else
				{
					var svm_string = document.createElement("input");
					svm_string.type = "text";
					svm_string.value = c.value;
				}
				svm_field.appendChild(svm_string);
				var svm_validity = document.createElement("span");
				svm_field.appendChild(svm_validity);
			}
			else if(c.type == "text")
			{
				var svm_label = document.createElement("div");
				svm_label.classList.add("label");
				svm_label.innerText = c.label;
				svm_field.appendChild(svm_label);
				var svm_validity = document.createElement("span");
				svm_field.appendChild(svm_validity);
				if(form.submitted != null)
				{
					var svm_string = document.createElement("pre");
					svm_string.innerText = c.value;
				}
				else
				{
					var svm_string = document.createElement("textarea");
					svm_string.rows = c.height;
					svm_string.cols = c.width;
				}
				svm_field.appendChild(svm_string);
			}
			svm_fields.appendChild(svm_field);
		}
		svm_formular.appendChild(svm_fields);
		if(form.submitted == null)
		{
			var svm_button = document.createElement("div");
			svm_button.classList.add("button");
			svm_button.innerText = "Submit";
			svm_button.svm_processor_id = form.processor;
			svm_button.svm_formular = form.fields;
			svm_button.addEventListener("click",function(event)
			{
				var svm_fields = event.target.parentNode.children[1];
				var svm_values = [];
				var svm_valid = true;
				var svm_index = 0;
				for(var c of event.target.svm_formular)
				{
					if(c.type == "checkbox")
					{
						svm_values.push(svm_fields.children[svm_index].children[0].checked);
					}
					else if(c.type == "selection")
					{
						svm_values.push(svm_fields.children[svm_index].children[1].value);
					}
					else if(c.type == "integer")
					{
						svm_fields.children[svm_index].children[2].classList.remove("invalid");
						var v = parseInt(svm_fields.children[svm_index].children[1].value);
						if(isNaN(v))
						{
							svm_fields.children[svm_index].children[2].classList.add("invalid");
							svm_valid=false;
						}
						else
						{
							if((c.min != null)&&(c.max != null))
							{
								if((c.min<=v)&&(v<=c.max))
								{
									svm_values.push(v);
								}
								else
								{
									svm_fields.children[svm_index].children[2].classList.add("invalid");
									svm_valid=false;
								}
							}
							else
							{
								svm_values.push(v);
							}
						}
					}
					else if(c.type == "string")
					{
						svm_fields.children[svm_index].children[2].classList.remove("invalid");
						if((c.min != null)&&(c.max != null))
						{
							var v = svm_fields.children[svm_index].children[1].value;
							if((c.min<=v.length)&&(v.length<=c.max))
							{
								svm_values.push(v);
							}
							else
							{
								svm_fields.children[svm_index].children[2].classList.add("invalid");
								svm_valid=false;
							}
						}
						else
						{
							svm_values.push(v);
						}
					}
					else if(c.type == "text")
					{
						svm_values.push(svm_fields.children[svm_index].children[2].value);
					}
					++svm_index;
				}
				if(svm_valid)
				{
					svm_controler.svm_send({command:"submit", identifier: event.target.svm_processor_id, values: svm_values});
				}
			});
			svm_formular.appendChild(svm_button);
		}
		return svm_formular;
	}
	svm_breakpoint(p,id,svm_view)
	{
		var pa = document.createElement("tr");
		pa.svm_identifier = p.identifier;
		var svm_delete = document.createElement("td");
		var svm_delete_button = document.createElement("div");
		svm_delete_button.classList.add("button");
		svm_delete_button.classList.add("delete");
		svm_delete_button.svm_breakpoint_id = p.identifier;
		svm_delete_button.svm_identifier = id;
		svm_delete_button.svm_view = svm_view;
		svm_delete_button.addEventListener("click",function(event) {
			if((event.target.parentNode.parentNode.children[2].children[1] != null)&&(event.target.parentNode.parentNode.children[2].children[1].childElementCount>2))
			{
				return;
			}
			event.target.svm_view.svm_box.svm_acknowledge();
			svm_controler.svm_send({ command: "event", identifier: event.target.svm_identifier, remove_breakpoint: event.target.svm_breakpoint_id});
		});
		svm_delete.appendChild(svm_delete_button);
		pa.appendChild(svm_delete);
		var svm_open = document.createElement("td");
		var svm_open_button = document.createElement("div");
		svm_open_button.classList.add("button");
		if(p.kernel != null)
		{
			svm_open_button.classList.add("open");
			svm_open_button.svm_breakpoint_id = p.identifier;
			svm_open_button.svm_identifier = id;
			svm_open_button.svm_kernel_id = p.kernel;
			svm_open_button.nom_processus = p.process_name;
			svm_open_button.nom_noyau = p.kernel_name;
			svm_open_button.svm_view = svm_view;
			svm_open_button.addEventListener("click",function(event)
			{
				event.target.svm_view.svm_box.svm_acknowledge();
				svm_interface.svm_open_kernel("P "+event.target.nom_processus.substring(8),event.target.svm_kernel_id,event.target.nom_noyau,null);
			});
		}
		else
		{
			svm_open_button.classList.add("cannot_open");
		}
		svm_open.appendChild(svm_open_button);
		pa.appendChild(svm_open);
		var svm_text = document.createElement("td");
		svm_text.classList.add("text");
		var verbatim = document.createElement("pre");
		var type = document.createElement("b");
		type.innerText = p.type;
		verbatim.appendChild(type);
		verbatim.innerHTML += ":\n" + p.message;
		svm_text.appendChild(verbatim);
		if(p.form != null)
		{
			svm_text.appendChild(svm_interface.svm_formular(p.form));
		}
		pa.appendChild(svm_text);
		return pa;
	}
	svm_open_breakpoints_list()
	{
		var svm_view = new SVM_ViewBreakpointsList();
		svm_controler.svm_add_view(svm_view);
		svm_controler.svm_add_index_function(svm_view,"breakpoints",svm_view.svm_box.svm_usable(),function(svm_view,dom,message)
		{
			svm_controler.svm_add_object_function(svm_view,message.identifiers[0],"event",svm_view.svm_box.svm_usable().children[0],function(svm_view,dom,message)
			{
				if(message.event.add_breakpoint != null)
				{
					var pa = svm_interface.svm_breakpoint(message.event.add_breakpoint,message.identifier,svm_view);
					dom.children[0].appendChild(pa);
					pa.scrollIntoView({ behavior: "smooth"});
					svm_view.svm_box.svm_notify();
				}
				if(message.event.remove_breakpoint != null)
				{
					for(var i = 0 ; i<dom.children[0].children.length ; i++)
					{
						if(dom.children[0].children[i].svm_identifier == message.event.remove_breakpoint)
						{
							dom.children[0].removeChild(dom.children[0].children[i]);
						}
					}
				}
				if(message.event.clear_form != null)
				{
					for(var i = 0 ; i<dom.children[0].children.length ; i++)
					{
						if(dom.children[0].children[i].svm_identifier == message.event.clear_form.identifier)
						{
							dom.children[0].children[i].children[2].children[1].innerHTML="";
							dom.children[0].children[i].children[2].children[1].appendChild(svm_interface.svm_formular(message.event.clear_form.form));
						}
					}
				}
			});
			svm_controler.svm_add_object_function(svm_view,message.identifiers[0],"object",svm_view.svm_box.svm_usable().children[0],function(svm_view,dom,message)
			{
				dom.innerHTML = "";
				var t = document.createElement("table");
				t.classList.add("breakpoints");
				for(var p of message.breakpoints)
				{
					var pa = svm_interface.svm_breakpoint(p,message.identifier,svm_view);
					t.appendChild(pa);
					svm_view.svm_box.svm_notify();
				}
				dom.appendChild(t);
				if(t.lastChild != null)
				{
					t.lastChild.scrollIntoView({ behavior: "smooth"});
				}
			});
			svm_controler.svm_send({ command: "object", identifier: message.identifiers[0] });
		});
		svm_controler.svm_send({ command: "index", type: "breakpoints" });
	}
	svm_plugins_filter(e)
	{
		var svm_filter = e.target.parentNode.children[0].value;
		var type = e.target.parentNode.children[1].value;
		for(var l of e.target.parentNode.parentNode.children[0].children[0].children)
		{
			if(l.children[0].nodeName != "TD") continue;
			l.style.display = "";
			if(((type=="")||(l.children[0].innerText.search(type)==0))&&((svm_filter=="")||(l.children[0].innerText.search(svm_filter)!=-1))) continue;
			l.style.display = "none";
		}
	}
	svm_display_memory_address(e)
	{
		var svm_address = parseInt(e.target.parentNode.children[0].value);
		if(isNaN(svm_address))
		{
			return;
		}
		if((svm_address>=0)&&(svm_address<e.target.parentNode.parentNode.children[0].children[0].children.length))
		{
			svm_interface.svm_add_memory_highlight(e.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view,e.target.parentNode.parentNode.children[0].children[0],{address:svm_address,size:1});
		}
	}
	svm_display_memory_current(e)
	{
		svm_controler.svm_send({ command: "event", identifier: e.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view.id, current: true});
	}
	svm_add_memory_highlight(v,t,p)
	{
		if(v.svm_pointers.length>0)
		{
			svm_interface.svm_remove_pointer_highligh(t,v.svm_pointers[v.svm_pointers.length-1]);
		}
		v.svm_pointers.push(p);
		svm_interface.svm_add_pointer_highlight(t,p);
	}
	svm_display_memory_highlight(e)
	{
		var v = e.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view;
		if(v.svm_pointers.length==0)
		{
			return;
		}
		e.target.parentNode.parentNode.children[0].children[0].children[v.svm_pointers[v.svm_pointers.length-1].address].scrollIntoView({behavior:"smooth"});
	}
	svm_remove_memory_highlight(e)
	{
		var v = e.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view;
		if(v.svm_pointers.length==0) return;
		svm_interface.svm_remove_pointer_highligh(e.target.parentNode.parentNode.children[0].children[0],v.svm_pointers[v.svm_pointers.length-1]);
		v.svm_pointers.pop();
		if(v.svm_pointers.length==0) return;
		svm_interface.svm_add_pointer_highlight(e.target.parentNode.parentNode.children[0].children[0],v.svm_pointers[v.svm_pointers.length-1]);
	}
	svm_reset_memory_highlight(e)
	{
		var v = e.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view;
		if(v.svm_pointers.length==0) return;
		svm_interface.svm_remove_pointer_highligh(e.target.parentNode.parentNode.children[0].children[0],v.svm_pointers[v.svm_pointers.length-1]);
		v.svm_pointers = [];
	}
	svm_remove_pointer_highligh(t,p)
	{
		if(p.address>=(t.children.length-1))
		{
			p.address = t.children.length-1;
			p.size = 0;
		}
		for(var a=p.address ; a<(p.address+p.size) ; a++)
		{
			t.children[a+1].children[0].classList.remove("pointer");
		}
	}
	svm_add_pointer_highlight(t,p)
	{
		if(p.address>=(t.children.length-1))
		{
			p.address = t.children.length-1;
			p.size = 0;
		}
		for(var a=p.address ; a<(p.address+p.size) ; a++)
		{
			t.children[a+1].children[0].classList.add("pointer");
		}
		t.children[p.address].scrollIntoView({behavior:"smooth"});
	}
	svm_lost_connection()
	{
		if(svm_controler.svm_end) return;
		if(svm_controler.svm_login)
		{
			var b = this.svm_desktop.svm_add_window("erreur","Error","<div>Connection to SVM lost!</div>",null,new SVM_PositionClever(this.svm_desktop,200,100,null),new SVM_SizeFixed(200,100));
		}
		else
		{
			svm_interface.svm_desktop.svm_remove_window(svm_interface.login_box);
			var b = this.svm_desktop.svm_add_window("erreur","Error","<div>Login failed!</div><div>You shall reload the page to get a new login.</div>",new SVM_Events(null,null,null,null,function(f) {f.svm_show();},null,null,function(f) { return false; }),new SVM_PositionClever(this.svm_desktop,200,100,null),new SVM_SizeFixed(200,100));
		}
		b.svm_notify();
	}
};

class SVM_ViewMachine
{
	constructor()
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("machine","Machine","<div>Status: <span>...</span></div><div>Version: <span>...</span></div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,200,100,null),new SVM_SizeMinimum(200,100),'<div class="help_content">This window shows the global status of the Simple Virtual Machine and its version.<br/>In case the machine is stuck, you can ask it to kill itself by a click on <div class="button line">Kill</div>.<hr/><div>Below the horizontal line there is a client setting, controlling when the user interface is refreshed by the virtual machine&nbsp;:<ul><li><div class="button line">Always</div> indicates the interface is refreshed for every change within the virtual machine (default behavior),</li><li><div class="button line">On breakpoints</div> indicates the interface is refreshed only when a breakpoint is reached by the virtual machine (useful when the web browser freezes between breakpoints)</li></ul></div></div>');
		this.svm_box.svm_view = this;
	}
	
};

class SVM_ViewSchedulersList
{
	constructor()
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("scheduler_list","Scheduler list","<div>...</div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,300,200,null), new SVM_SizeFixedWidthMinimumHeight(300,100),'<div class="help_content">This window lists all available schedulers in the Simple Virtual Machine.<br/>You can click on a scheduler name to see details on the corresponding scheduler.</div>');
		this.svm_box.svm_view = this;
	}
};

class SVM_ViewScheduler
{
	constructor(svm_name)
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("scheduler","<b>"+svm_name+"</b>","<pre style=\"margin:0\">...</pre>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,300,200,null),new SVM_SizeMinimum(300,200),'<div class="help_content">This window contains the textual representation of the scheduler state, and potentially the processes it manages.</div>');
		this.svm_box.svm_view = this;
	}
};

class SVM_ViewPlugins
{
	constructor()
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("plugins","Plugins","<div style=\"overflow:scroll; height:500; width:700\"><table></table></div><div class=\"tool_bar_bottom\"><input type=\"text\" onkeydown=\"if(event.code=='Enter') { svm_interface.svm_plugins_filter(event); }\" onclick='if(event.ctrlKey) { event.target.value=\"\"; }'></input> in <select><option value=''>all</option><option value='TYPE'>types</option><option value='(SYSTEM )?(WAITING )?INSTRUCTION'>instructions</option><option value='INTERRUPTION'>interruptions</option><option value='FUNCTION'>functions</option><option value='STRUCT'>structures</option><option value='SCHEDULER'>schedulers</option><option value='SEQUENCER'>sequencers</option><option value='OPTION'>options</option><option value='ARGUMENT'>arguments</option></select><div class=\"button line\" onclick=\"svm_interface.svm_plugins_filter(event)\">Filter</div></div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,700,500,null),new SVM_SizeMinimum(700,500),'<div class="help_content">This window lists all objects declared in plugins.<br/>The table can be scrolled to display all elements.<br/>At the bottom of the window, a filter can be applied on the plugin object definitions for a fast search:<ul><li>The input can contain a regular expression (ECMAScript1 compatible) and an empty input will select all lines,</li><li>A control-click on the input will clear it,</li><li>The selection list can perform the search on a dedicated type of plugin objects.</li></ul>Once the filter criteria is entered, a click on <div class="button line">Filter</div> will apply the filter.</div>');
		this.svm_box.svm_view = this;
		this.svm_new_view = true;
	}
};

class SVM_ViewEvents
{
	constructor()
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("events","Events","<div class=\"tool_bar_top\"><pre style=\"display: inline;\"></pre></div><div style=\"position: relative; overflow:scroll; height:500; width:700\"></div><div class=\"tool_bar_bottom\"><div class=\"button line\" onclick=\"event.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view.svm_sequence_diagram.svm_zoom_increase();\">Zoom in</div><div class=\"button line\" onclick=\"event.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view.svm_sequence_diagram.svm_zoom_decrease();\">Zoom out</div><div class=\"button line\" style=\"margin-left: 10px\" onclick=\"event.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view.svm_sequence_diagram.svm_zoom_reset();\">Reset zoom</div><span style=\"margin-left:30px;\">Marker color:</span><select onchange=\"event.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view.svm_sequence_diagram.svm_markers_color=event.target.value;\"><option value=\"#DD0000\">red</option><option value=\"#00DD00\">green</option><option value=\"#0000DD\">blue</option><option value=\"#DDDD00\">yellow</option><option value=\"#DD00DD\">magenta</option><option value=\"#00DDDD\">cyan</option><option value=\"#FFFFFF\">white</option></select><div class=\"button line\" style=\"margin-left: 10px;\" onclick=\"event.target.parentNode.parentNode.parentNode.parentNode.svm_window.svm_view.svm_sequence_diagram.svm_markers_reset();\">Clear markers</div></div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,700,500,null),new SVM_SizeMinimum(700,500),'<div class="help_large_content">This window shows the sequence diagram of events exchanged between:<ul><li>The system,</li><li>Each scheduler,</li><li>Each process.</li></ul>The name of the participant is written at the top, and the thick vertical line associated to the name will represent it along time.<br/>Exchanged events are represented by horizontal arrows, with the content of the event:<ul><li>The arrow starts at the time the event is emitted by its source,</li><li>It ends at the time the event is received by its target,</li><li>When an arrow is not complete, it means the event is not yet received by its target.</li></ul>When a click is done on the sequence diagram outside a participant bar, a colored marker can be set or reset to indicate a particular time in the sequence, or a participant when the click is done on a participant name.<br/>The toolbar at the bottom of the window allows you to:<ul><li>Increase the diagram size by a click on <div class="button line">Zoom in</div>,</li><li>Decrease the diagram size by a click on <div class="button line">Zoom out</div>,</li><li>And reset the diagram to its original size by a click on <div class="button line">Reset zoom</div>,</li><li>Change the color of next placed markers using the color list,</li><li>Remove all markers by a click on <div class="button line">Clear markers</div>.</li></ul></div>');
		this.svm_box.svm_view = this;
		this.svm_new_view = true;
	}
};

class SVM_ViewProcessesList
{
	constructor()
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("process_list","Process list","<div>...</div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,300,200,null), new SVM_SizeFixedWidthMinimumHeight(300,100),'<div class="help_content">This window lists all existing processes within the Simple Virtual Machine with their state.<br/>You can click on a process name to see details of the corresponding process.</div>');
		this.svm_box.svm_view = this;
	}
};

class SVM_ViewProcess
{
	constructor(svm_name)
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("process","<b>"+svm_name+"</b>","<pre style=\"margin:0\">...</pre>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,300,200,null),new SVM_SizeMinimum(300,200),'<div class="help_content">This window shows the process state and its components:<ul><li><div class="button line">Sequencer</div> shows the process\' sequencer state,</li><li><div class="button line">Current kernel</div> shows the kernel currently selected by the sequencer for svm_run,</li><li>The full kernel list of the process is shown, and you can click on a kernel name to see details of the corresponding kernel.</li></ul></div>');
		this.svm_box.svm_view = this;
	}
};

class SVM_ViewSequencer
{
	constructor(svm_name,svm_box)
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("sequencer","<b>Sequencer</b> - "+svm_name,"<pre style=\"margin:0\">...</pre>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionBox(svm_box),new SVM_SizeMinimum(300,200),'<div class="help_content">This window contains the textual representation of the sequencer state, and potentially the kernels it manages.</dev>');
		this.svm_box.svm_view = this;
	}
};

class SVM_ViewKernelsList
{
	constructor()
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("kernel_list","Kernel list","<div>...</div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,400,200,null), new SVM_SizeFixedWidthMinimumHeight(400,100),'<div class="help_content">This window lists all existing kernels within the Simple Virtual Machine.<br/>You can click on a kernel name to see details on the corresponding kernel.</div>');
		this.svm_box.svm_view = this;
	}
};

class SVM_ViewKernel
{
	constructor(svm_process,svm_name,svm_box)
	{
		var svm_position = null;
		if(svm_box == null)
		{
			svm_position = new SVM_PositionClever(svm_interface.svm_desktop,400,200,null);
		}
		else
		{
			svm_position = new SVM_PositionBox(svm_box);
		}
		this.svm_box = svm_interface.svm_desktop.svm_add_window("kernel","<b>"+svm_name+"</b> - "+svm_process,"...",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),svm_position,new SVM_SizeBounded(300,100,300,200),'<div class="help_content">This windows shows the global state of the kernel and its flags.<br/><ul><li><div class="button line">Processor</div> shows the kernel\'s processor state and allows interactions with the processor,</li><li><div class="button line">Memory</div> shows the kernel\'s memory content.</li></ul></div>');
		this.svm_box.svm_view = this;
	}
};

class SVM_ViewProcessor
{
	constructor(svm_process,svm_kernel,id,svm_box)
	{
		var svm_position = null;
		if(svm_box == null)
		{
			svm_position = new SVM_PositionClever(svm_interface.svm_desktop,700,500,null);
		}
		else
		{
			svm_position = new SVM_PositionBox(svm_box);
		}
		var svm_name = "Processor - "+svm_kernel+" - "+svm_process;
		this.svm_box = svm_interface.svm_desktop.svm_add_window("processor","<b>Processor</b> - "+svm_kernel+" - "+svm_process,"<div>...</div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),svm_position,new SVM_SizeFixed(700,500),'<div class="help_large_content">This window shows the processor state:<ul><li>The first part is the processor state with registers. On the current instruction, you can click on <div class="button line">Code</div> to open the code with a pointer indicating which instruction is the current one or you can click on <div class="button line">Explain</div> to see computation of all variables part of the current instruction,</li><li>The second part is the processor return stack. For each state, you can click on <div class="button line">Code</div> to open the code with a static pointer to the current instruction on this parent function call,</li><li>On each interruption handler, you can click on <div class="button line">Code</div> to open the code with a static pointer indicating where the interruption handler is located.</li></ul>This window also allows interactions from you:<ul><li><div class="button processor_pause line"></div> will ask the processor to stop the execution on the next instruction,</li><li><div class="button processor_instruction line"></div> will ask the processor to run only one instruction,</li><li><div class="button processor_function line"></div> will ask the processor to run the code until it returns to the current function or exits the current function,</li><li><div class="button processor_run line"></div> will ask the processor to resume the execution,</li><li><div class="button processor_return line"></div> will ask the processor to run the code until the state becomes the current state.</li></ul>In all cases, the execution will stop if the processor reaches a break point.</div>');
		this.svm_box.svm_view = this;
		this.id = id;
		this.svm_name = svm_name;
		this.svm_breakpoints = [];
	}
};
class SVM_ViewMemory
{
	constructor(svm_process,svm_kernel,id,svm_box)
	{
		var svm_position = null;
		if(svm_box == null)
		{
			svm_position = new SVM_PositionClever(svm_interface.svm_desktop,700,500,null);
		}
		else
		{
			svm_position = new SVM_PositionBox(svm_box);
		}
		this.svm_box = svm_interface.svm_desktop.svm_add_window("memory","<b>Memory</b> - "+svm_kernel+" - "+svm_process,"<div style=\"overflow:scroll; width:700; height:500\">...</div><div class='tool_bar_bottom'>Address: <input type='number' svm_minimum=0 svm_maximum=0 size=5 style='width:70px;' onkeydown=\"if(event.code=='Enter') { svm_interface.svm_display_memory_address(event); }\" onclick='if(event.ctrlKey) { event.target.value=\"\"; }'></input><div class='button line' onclick='svm_interface.svm_display_memory_address(event)'>Display</div><div class=\"button line\" onclick='event.target.parentNode.parentNode.children[0].children[1].scrollIntoView({ behavior: \"smooth\"});'>Aliases</div><div class=\"button line\" onclick='svm_interface.svm_display_memory_current(event)'>P</div><div class='button line' style='margin-left: 10px' onclick='svm_interface.svm_display_memory_highlight(event)'>Focus</div><div class='button line' onclick='svm_interface.svm_remove_memory_highlight(event)'>Back</div><div class='button line' style='margin-left: 3px' onclick='svm_interface.svm_reset_memory_highlight(event)'>Clear</div></div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),svm_position,new SVM_SizeMinimum(700,500),'<div class="help_large_content">This window shows the memory state.<br/>For each possible address, it shows:<ul><li>Its address when it is defined,</li><li>Its type when it is defined,</li><li>Its value when it is initialised. When the processor linked to this memory is stopped thanks to a break point, a click on the value will refresh the value from the memory. When the value is a pointer, a click on <div class="button line follow"></div> will highlight and show the referenced memory zone, if accessible in the view.</li></ul>At the bottom of the window, a fast search is available:<ul><li>By memory address:<ul><li>Once the numeric address is entered, a click on <div class="button line">Display</div> located after the address field will highlight and show the corresponding address in the main view if available,</li><li>A control-click on the address input will clear the written address;</li></ul></li><li>By alias:<ul><li>Once the alias is selected in the list, the corresponding memory zone will be shown and highlighted in the main view if available;</li></ul></li><li>From the current memory pointer by a click on <div class="button line">P</div>.</li></ul>A click on <div class="button line">Focus</div> will show the currently highlighted memory zone if any, or the selected alias otherwise.<br/>A click on <div class="button line">Back</div> will show the previous highlighted memory zone if any.<br/>A click on <div class="button line">Clear</div> will clear the memory zone highlight and its history.</div>');
		this.svm_box.svm_view = this;
		this.svm_new_view = true;
		this.svm_pointers = [];
		this.id = id;
	}
};
class SVM_ViewCode
{
	constructor(svm_name,svm_code_id,svm_processor_id,svm_current,svm_cursor,svm_breakpoints,svm_box)
	{
		var svm_position = null;
		this.svm_size = 200;
		if(svm_box == null)
		{
			svm_position = new SVM_PositionClever(svm_interface.svm_desktop,700,this.svm_size,null);
		}
		else
		{
			svm_position = new SVM_PositionBox(svm_box);
		}
		this.svm_box = svm_interface.svm_desktop.svm_add_window("code",svm_name,'<div class="tool_bar_top"></div><div style="overflow:scroll; width:700; height:'+this.svm_size+'; min-height:200px; resize: vertical;">...</div><div class="tool_bar_bottom"><input type="checkbox" checked="false" onchange="svm_interface.svm_code_cursor_scroll(event.target.parentNode.parentNode)"></input>Auto-scroll to <div class="button line" onclick="svm_interface.svm_code_cursor_scroll(event.target.parentNode.parentNode,true)">Current</div> with <select onchange="svm_interface.svm_code_cursor_scroll(event.target.parentNode.parentNode)"><option value="0">no instructions</option><option value="1">1 instruction</option><option value="2">2 instructions</option><option value="3">3 instructions</option><option value="5" selected="selected">5 instructions</option><option value="10">10 instructions</option><option value="20">20 instructions</option></select> above</div><div class="tool_bar_bottom">Display <select onchange="svm_interface.svm_display_code(event.target.parentNode.parentNode,event.target.value);event.target.children[0].selected=true;"><option value="0">Labels and symbols...</option></select></div>',new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),svm_position,new SVM_SizeMinimum(700,this.svm_size),'<div class="help_large_content">This window shows the code:<ul><li>The top toolbar displays some extra informations on the currently pointed instruction;</li><li>The main body displays the instructions. For each instruction:<ul><li>The first part on the left allows you to set or reset a break point on the instruction. When the break point is active, a <table style="display:inline-block; border:none;"><tr><td style="display:inline-block; border:none;" class="break active"/></tr></table> is displayed,</li><li>The second part indicates which instruction the processor is executing. When the window shows a static code pointer, a <table style="display:inline-block; border:none;"><tr><td style="display:inline-block; border:none;" class="cursor index"/></tr></table> is shown, and this pointer will not move. Otherwise a <table style="display:inline-block; border:none;"><tr><td style="display:inline-block; border:none;" class="cursor active"/></tr></table> is shown, and this pointer will move during processor run,</li><li>The last part is the instruction text;</li></ul></li><li>The first bottom toolbar allows you to:<ul><li>Auto-scroll the code to the run pointer when a break point is reached, with some context instructions above the pointer,</li><li>Scroll manually to the execution pointer with its context by a click on <div class="button line">Current</div>,</li><li>Change the number of seen instructions by resizing the code view using its bottom right corner;</li></ul></li><li>The second bottom toolbar allows you to display a specific label or symbol within the code.</li></ul></div>');
		this.svm_box.svm_view = this;
		this.svm_new_view = true;
		this.code = svm_code_id;
		this.svm_cursor = svm_cursor;
		this.svm_current = svm_current;
		this.svm_breakpoints = svm_breakpoints;
	}
};
class SVM_ViewBreakpointsList
{
	constructor()
	{
		this.svm_box = svm_interface.svm_desktop.svm_add_window("breakpoint_list","Breakpoint list","<div style=\"overflow:auto; min-height:100; max-height:800; resize: vertical;\">...</div>",new SVM_Events(null,function(f) { svm_controler.svm_remove_view(f.svm_view); },null,null,null,null,null),new SVM_PositionClever(svm_interface.svm_desktop,400,200,null), new SVM_SizeFixedWidthMinimumHeight(400,100),'<div class="help_large_content">This windows lists all reached break points by processors during code execution.<br/>For each break point:<ul><li><table class="breakpoints" style="display:inline-block"><tr style="display:inline-block"><td style="display:inline-block;padding:0px;"><div class="button delete line"></div></td></tr></table> will remove the break point from the list unless a non-submitted form is present in the break point,</li><li><table class="breakpoints" style="display:inline-block"><tr style="display:inline-block"><td style="display:inline-block;padding:0px;"><div class="button open line"></div></td></tr></table> will open the kernel window where the break point has been reached,</li><li>the last part will display the break point category, location and message.</li></ul>When a form is associated to a break point, this form is displayed under the break point message. Each field can interact as usual inputs, and a click on <div class="button line">Submit</div> will:<ol><li>perform basic checks on the inputs and indicate the invalid fields with a <div class="form" style="display:inline-block;border:none;"><div class="invalid" style="display:inline-block"></div></div>,</li><li>send the data to the Simple Virtual Machine when all inputs are valid.</li></ol></div>');
		this.svm_box.svm_view = this;
	}
}
