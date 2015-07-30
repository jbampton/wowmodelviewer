#include "database.h"

#include "globalvars.h"
#include "modelviewer.h"

#include "logger/Logger.h"


_DATABASE_API_ ItemDatabase		items;
_DATABASE_API_ std::vector<NPCRecord> npcs;

// --
_DATABASE_API_ HelmGeosetDB		helmetdb;
_DATABASE_API_ StartOutfitDB		startdb;
_DATABASE_API_ ItemSetDB			setsdb;

//--
_DATABASE_API_ LightSkyBoxDB			skyboxdb;
_DATABASE_API_ CamCinematicDB		 camcinemadb;

// --
// CAMCINEMADB.H
CamCinematicDB::Record CamCinematicDB::getByCamModel(wxString fn)
{
	LOG_INFO << "Searching for CamModel...";
	// Brute force search for now
	for (Iterator i=begin(); i!=end(); ++i)
	{
		wxString str(i->getString(CamModel));
		LOG_INFO << "CamModel:" << str.c_str() << "VS" <<  fn.c_str();
		if(str.IsSameAs(fn, false) == true)
			return (*i);
	}
	throw NotFound();
}

// Head and Helmet display info
HelmGeosetDB::Record HelmGeosetDB::getById(unsigned int id)
{
	for(Iterator i=begin(); i!=end(); ++i)
	{
		if (i->getUInt(TypeID)==id)
			return (*i);
	}
	throw NotFound();
}
// --


// --
// ITEMDB.H
//
// --------------------------------
// Item Database Stuff
// --------------------------------

ItemSetDB::Record ItemSetDB::getById(unsigned int id)
{
	for(Iterator i=begin(); i!=end(); ++i)
	{
		if (i->getUInt(SetID)==id)
			return (*i);
	}
	throw NotFound();
}

void ItemSetDB::cleanup(ItemDatabase &p_itemdb)
{
	for(Iterator i=begin(); i!=end(); ++i) {
		for (size_t j=0; j<NumItems; j++) {
			int id = i->getUInt(ItemIDBaseV400+j);
			if (id > 0) {
				const ItemRecord &r = p_itemdb.getById(id);
				if (r.type > 0) {
					avail.insert(i->getUInt(SetID));
					break;
				}
			}
		}
	}
}

bool ItemSetDB::available(unsigned int id)
{
	return (avail.find(id)!=avail.end());
}


StartOutfitDB::Record StartOutfitDB::getById(unsigned int id)
{
	for(Iterator i=begin(); i!=end(); ++i)
	{
		if (i->getUInt(StartOutfitID)==id)
			return (*i);
	}
	throw NotFound();
}



////////////////////
ItemRecord::ItemRecord(const std::vector<std::string> & vals)
  : id(0), itemclass(0), subclass(0), type(0), model(0), sheath(0), quality(0)
{
  if(vals.size() < 7)
      return;

  id = atoi(vals[0].c_str());
  type = atoi(vals[2].c_str());
  itemclass = atoi(vals[3].c_str());
  subclass = atoi(vals[4].c_str());
  model = 1;
  quality = atoi(vals[6].c_str());
  switch(atoi(vals[5].c_str()))
  {
    case SHEATHETYPE_MAINHAND: sheath = ATT_LEFT_BACK_SHEATH; break;
    case SHEATHETYPE_LARGEWEAPON: sheath = ATT_LEFT_BACK; break;
    case SHEATHETYPE_HIPWEAPON: sheath = ATT_LEFT_HIP_SHEATH; break;
    case SHEATHETYPE_SHIELD: sheath = ATT_MIDDLE_BACK_SHEATH; break;
    default: sheath = SHEATHETYPE_NONE;
  }
  name = wxConvLocal.cWC2WX(wxConvUTF8.cMB2WC(wxString(vals[1].c_str(), wxConvUTF8).mb_str()));
}

// Alfred. prevent null items bug.
ItemDatabase::ItemDatabase()
{
	ItemRecord all;
	all.name=wxT("---- None ----");
	all.type=IT_ALL;

	items.push_back(all);
}

const ItemRecord& ItemDatabase::getById(int id)
{
  for (std::vector<ItemRecord>::iterator it=items.begin();  it!=items.end(); ++it)
  {
    if(it->id == id)
      return *it;
  }
  return items[0];
}

// ============================================================
// =============================================================

NPCRecord::NPCRecord(wxString line)
    : id(0), model(0), type(0)
{
	if (line.Len() <= 3)
	    return;
	id = wxAtoi(line.BeforeFirst(','));
	line = line.AfterFirst(',');
	model = wxAtoi(line.BeforeFirst(','));
	line = line.AfterFirst(',');
	type = wxAtoi(line.BeforeFirst(','));
	name = line.AfterFirst(',');
}


NPCRecord::NPCRecord(const std::vector<std::string> & vals)
    : id(0), model(0), type(0)
{
  if(vals.size() < 4)
    return;

  id = atoi(vals[0].c_str());
  model = atoi(vals[1].c_str());
  type = atoi(vals[2].c_str());
  name = wxConvLocal.cWC2WX(wxConvUTF8.cMB2WC(wxString(vals[3].c_str(), wxConvUTF8).mb_str()));
}
