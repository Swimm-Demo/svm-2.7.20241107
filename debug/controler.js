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

class SVM_Function
{
	constructor(svm_view,dom,svm_function)
	{
		this.svm_view = svm_view;
		this.dom = dom;
		this.svm_function = svm_function;
	}
	svm_call(message)
	{
		this.svm_function(this.svm_view,this.dom,message);
	}
};

class SVM_Controler
{
	constructor(socket)
	{
		this.svm_title = '';
		this.svm_main_key = '';
		this.svm_cipher_keys = [];
		this.svm_active_cipher = false;
		this.svm_views = [];
		this.socket = socket;
		this.svm_login = false;
		this.svm_end = false;
		this.socket.addEventListener('message',function(event) { svm_controler.svm_svm_event(event.data); });
		this.socket.addEventListener('close',function(event) { svm_interface.svm_lost_connection(); });
	}
	svm_add_view(svm_view)
	{
		this.svm_views.push(svm_view);
		svm_view.svm_object_functions = new Map();
		svm_view.svm_index_functions = new Map();
		svm_view.svm_client_functions = null;
	}
	svm_remove_view(svm_view)
	{
		for(var i=0 ; i<this.svm_views.length ; ++i)
		{
			if(this.svm_views[i] === svm_view)
			{
				this.svm_views.splice(i,1);
				break;
			}
		}
	}
	svm_add_object_function(svm_view,svm_object,svm_event,dom,svm_function)
	{
		for(var i=0 ; i<this.svm_views.length ; ++i)
		{
			if(this.svm_views[i] === svm_view)
			{
				if(!svm_view.svm_object_functions.has(svm_object))
				{
					svm_view.svm_object_functions.set(svm_object,new Map());
				}
				svm_view.svm_object_functions.get(svm_object).set(svm_event,new SVM_Function(svm_view,dom,svm_function));
				break;
			}
		}
	}
	svm_add_index_function(svm_view,type,dom,svm_function)
	{
		for(var i=0 ; i<this.svm_views.length ; ++i)
		{
			if(this.svm_views[i] === svm_view)
			{
				svm_view.svm_index_functions.set(type,new SVM_Function(svm_view,dom,svm_function));
				break;
			}
		}
	}
	svm_set_client_function(svm_view,dom,svm_function)
	{
		for(var i=0 ; i<this.svm_views.length ; ++i)
		{
			if(this.svm_views[i] === svm_view)
			{
				svm_view.svm_client_functions = new SVM_Function(svm_view,dom,svm_function);
				break;
			}
		}
	}
	svm_svm_event(svm_raw_message)
	{
		if(this.svm_active_cipher)
		{
			svm_raw_message = svm_decipher(hex2rstr(svm_raw_message),this.svm_cipher_keys);
		}
		var message = JSON.parse(svm_raw_message);
		if((message.command==="object") || (message.command==="delete") || (message.command==="event"))
		{
			for(var i=0 ; i<this.svm_views.length ; ++i)
			{
				if(this.svm_views[i].svm_object_functions.has(message.identifier))
				{
					if(this.svm_views[i].svm_object_functions.get(message.identifier).has(message.command))
					{
						this.svm_views[i].svm_object_functions.get(message.identifier).get(message.command).svm_call(message);
					}
				}
			}
		}
		else if(message.command==="index")
		{
			for(var i=0 ; i<this.svm_views.length ; ++i)
			{
				if(this.svm_views[i].svm_index_functions.has(message.type))
				{
					this.svm_views[i].svm_index_functions.get(message.type).svm_call(message);
				}
			}
		}
		else if(message.command==="client")
		{
			for(var i=0 ; i<this.svm_views.length ; ++i)
			{
				if(this.svm_views[i].svm_client_functions != null)
				{
					this.svm_views[i].svm_client_functions.svm_call(message);
				}
			}
		}
		else if(message.command==="login")
		{
			this.login(svm_interface,message);
		}
		else if(message.command==="title")
		{
			this.svm_title(svm_interface,message);
		}
		else if(message.command==="cipher")
		{
			this.svm_cipher_keys = svm_derivated_keys(this.svm_main_key,message.challenges);
			this.svm_active_cipher = true;
		}
		else if(message.command==="end")
		{
			this.svm_end = true;
		}
	}
	svm_send(message)
	{
		if(this.svm_active_cipher)
		{
			this.socket.send(rstr2hex(svm_cipher(JSON.stringify(message),this.svm_cipher_keys)));
		}
		else
		{
			this.socket.send(JSON.stringify(message));
		}
	}
};
