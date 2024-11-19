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

#include <src/machine/elements/valeur/code.h>

using namespace SVM::Machine::Element::Valeur;

SVM_Debugueur::JSON_Interne::ObjetSP PiegeCode::mise_a_jour()
{
	auto code = _code.lock();
	if(not code)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	auto o = SVM_Debugueur::JSON::objet();
	o->ajoute("name",SVM_Debugueur::JSON::chaine(code->_nom));
	auto etiquettes_inverses = Code::inverse(code->_ancres_etiquettes);
	auto symboles_inverses = Code::inverse(code->_ancres_symboles);
	auto t = SVM_Debugueur::JSON::tableau();
	size_t adresse = 0;
	for(const auto& i: code->_instructions)
	{
		auto oi = SVM_Debugueur::JSON::objet({ { "text" , SVM_Debugueur::JSON::chaine(i->texte_html()) } , { "extra" , SVM_Debugueur::JSON::chaine(i->extra_html(adresse)) } });
		auto e = etiquettes_inverses.equal_range(adresse);
		bool etiquette = false;
		std::ostringstream oss;
		oss << "<pre class=\"code labels\">";
		for(auto& ee = e.first ; ee!=e.second ; ++ee)
		{
			oss << "<span class=\"instruction\">:label</span> " << ee->second << std::endl;
			etiquette = true;
		}
		auto s = symboles_inverses.equal_range(adresse);
		for(auto& ss = s.first ; ss!=s.second ; ++ss)
		{
			oss << "<span class=\"instruction\">:symbol</span> " << ss->second << std::endl;
			etiquette = true;
		}
		oss << "</pre>";
		if(etiquette)
		{
			oi->ajoute("labels",SVM_Debugueur::JSON::chaine(oss.str()));
		}
		t->ajoute(oi);
		++adresse;
	}
	auto oi = SVM_Debugueur::JSON::objet();
	bool etiquette = false;
	std::ostringstream oss;
	oss << "<pre class=\"code labels\">";
	auto e = etiquettes_inverses.equal_range(adresse);
	for(auto& ee = e.first ; ee!=e.second ; ++ee)
	{
		oss << "<span class=\"instruction\">:label</span> " << ee->second << std::endl;
		etiquette = true;
	}
	auto s = symboles_inverses.equal_range(adresse);
	for(auto& ss = s.first ; ss!=s.second ; ++ss)
	{
		oss << "<span class=\"instruction\">:symbol</span> " << ss->second << std::endl;
		etiquette = true;
	}
	oss << "</pre>";
	if(etiquette)
	{
		oi->ajoute("labels",SVM_Debugueur::JSON::chaine(oss.str()));
	}
	t->ajoute(oi);
	o->ajoute("instructions",t);
	auto ee = SVM_Debugueur::JSON::tableau();
	for(const auto& ae : code->_ancres_etiquettes)
	{
		ee->ajoute(SVM_Debugueur::JSON::objet({ { "name" , SVM_Debugueur::JSON::chaine(ae.first) } , { "address" , SVM_Debugueur::JSON::entier(ae.second) } }));
	}
	o->ajoute("labels",ee);
	auto ss = SVM_Debugueur::JSON::tableau();
	for(const auto& as : code->_ancres_symboles)
	{
		ss->ajoute(SVM_Debugueur::JSON::objet({ { "name" , SVM_Debugueur::JSON::chaine(as.first) } , { "address" , SVM_Debugueur::JSON::entier(as.second) } }));
	}
	o->ajoute("symbols",ss);
	return o;
}
