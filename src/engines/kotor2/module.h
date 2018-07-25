/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names
 * can be found in the AUTHORS file distributed with this source
 * distribution.
 *
 * xoreos is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * xoreos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xoreos. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 *  The context needed to run a Star Wars: Knights of the Old Republic II - The Sith Lords module.
 */

#ifndef ENGINES_KOTOR2_MODULE_H
#define ENGINES_KOTOR2_MODULE_H

#include <list>
#include <set>

#include "src/common/scopedptr.h"
#include "src/common/ustring.h"
#include "src/common/changeid.h"
#include "src/common/configman.h"

#include "src/aurora/ifofile.h"

#include "src/aurora/nwscript/objectref.h"

#include "src/events/types.h"

#include "src/engines/kotor2/objectcontainer.h"
#include "src/engines/kotor2/object.h"

#include "src/engines/kotor2/gui/dialog.h"

namespace Engines {

class Console;

namespace KotOR2 {

class Area;
class Creature;
class IngameGUI;

class Module : public KotOR2::Object, public KotOR2::ObjectContainer {
public:
	Module(::Engines::Console &console);
	~Module();

	/** Clear the whole context. */
	void clear();

	// .--- Module management
	/** Is a module currently loaded and ready to run? */
	bool isLoaded() const;
	/** Is a module currently running? */
	bool isRunning() const;

	/** Load a module. */
	void load(const Common::UString &module, const Common::UString &entryLocation = "",
	          ObjectType entryLocationType = kObjectTypeAll);
	/** Use this character as the player character. */
	void usePC(Creature *pc);
	/** Exit the currently running module. */
	void exit();
	// '---

	// .--- Information about the current module
	/** Return the IFO of the currently loaded module. */
	const Aurora::IFOFile &getIFO() const;

	/** Return the module's name. */
	const Common::UString &getName() const;
	// '---

	// .--- Elements of the current module
	/** Return the area the PC is currently in. */
	Area *getCurrentArea();
	/** Return the currently playing PC. */
	Creature *getPC();
	/** Return a map between surface type and whether it is walkable. */
	const std::vector<bool> &getWalkableSurfaces() const;
	// '---

	// .--- Interact with the current module
	/** Show the ingame main menu. */
	void showMenu();
	// '---

	void delayScript(const Common::UString &script,
	                 const Aurora::NWScript::ScriptState &state,
	                 Aurora::NWScript::Object *owner, Aurora::NWScript::Object *triggerer,
	                 uint32 delay);

	// .--- PC management
	/** Move the player character to this position within the current area. */
	void movePC(float x, float y, float z);
	/** Move the player character to this object within this area. */
	void movePC(const Common::UString &module, const Common::UString &object, ObjectType type = kObjectTypeAll);
	/** Notify the module that the PC was moved. */
	void movedPC();
	// '---

	// .--- Static utility methods
	static Common::UString getName(const Common::UString &module);
	// '---

	// .--- Module main loop (called by the Game class)
	/** Enter the loaded module, starting it. */
	void enter();
	/** Leave the running module, quitting it. */
	void leave();

	void clickObject(Object *object);

	/** Add a single event for consideration into the event queue. */
	void addEvent(const Events::Event &event);
	/** Process the current event queue. */
	void processEventQueue();
	// '---

	void toggleFreeRoamCamera();
	void toggleWalkmesh();
	void toggleTriggers();
	void startConversation(const Common::UString &name, Aurora::NWScript::Object *owner = 0);

	void playAnimationOnActiveObject(const Common::UString &baseAnim,
	                                 const Common::UString &headAnim);

private:
	enum ActionType {
		kActionNone   = 0,
		kActionScript = 1
	};

	struct Action {
		ActionType type;

		Common::UString script;

		Aurora::NWScript::ScriptState state;
		Aurora::NWScript::ObjectReference owner;
		Aurora::NWScript::ObjectReference triggerer;

		uint32 timestamp;

		bool operator<(const Action &s) const;
	};

	typedef std::list<Events::Event> EventQueue;
	typedef std::multiset<Action> ActionQueue;


	::Engines::Console *_console;

	bool _hasModule; ///< Do we have a module?
	bool _running;   ///< Are we currently running a module?

	/** Resources added by the current module. */
	std::list<Common::ChangeID> _resources;

	/** The current module's IFO. */
	Aurora::IFOFile _ifo;

	Common::ScopedPtr<Creature> _pc; ///< The player character we use.

	/** The current texture pack. */
	int _currentTexturePack;
	/** Resources added by the current texture pack. */
	Common::ChangeID _textures;

	bool _exit; ///< Should we exit the module?

	Common::UString _module;    ///< The current module's name.
	Common::UString _newModule; ///< The module we should change to.

	/** The tag of the object in the start location for this module. */
	Common::UString _entryLocation;
	/** The type(s) of the object in the start location for this module. */
	ObjectType      _entryLocationType;

	Common::ScopedPtr<Area> _area; ///< The current module's area.
	Common::ScopedPtr<DialogGUI> _dialog; ///< Conversation/cutscene GUI.

	EventQueue  _eventQueue;
	ActionQueue _delayedActions;

	bool _freeCamEnabled;
	uint32 _prevTimestamp;
	float _frameTime;
	float _forwardBtnPressed;
	float _backwardsBtnPressed;
	bool _pcRunning;
	bool _inDialog;
	float _cameraHeight;

	Common::ScopedPtr<Engines::KotOR2::IngameGUI> _ingame; ///< The ingame gui.

	// Surface types
	/** A map between surface type and whether it is walkable. */
	std::vector<bool> _walkableSurfaces;

	// .--- Unloading
	/** Unload the whole shebang.
	 *
	 *  @param completeUnload Also unload the PC and texture packs.
	 *                        true:  completely quit the module
	 *                        false: the PC can be transferred to a new module.
	 */
	void unload(bool completeUnload = true);

	void unloadResources();
	void unloadPC();
	void unloadIFO();
	void unloadArea();
	void unloadTexturePack();
	// '---

	// .--- Loading
	void load();

	void loadResources();
	void loadIFO();
	void loadArea();
	void loadTexturePack();
	/** Load the surface types. */
	void loadSurfaceTypes();
	// '---

	/** Load the actual module. */
	void loadModule(const Common::UString &module, const Common::UString &entryLocation,
	                ObjectType entryLocationType);
	/** Schedule a change to a new module. */
	void changeModule(const Common::UString &module, const Common::UString &entryLocation,
	                  ObjectType entryLocationType);
	/** Actually replace the currently running module. */
	void replaceModule();

	bool getObjectLocation(const Common::UString &object, ObjectType location,
	                       float &entryX, float &entryY, float &entryZ, float &entryAngle);
	bool getEntryObjectLocation(float &entryX, float &entryY, float &entryZ, float &entryAngle);
	void getEntryIFOLocation(float &entryX, float &entryY, float &entryZ, float &entryAngle);

	void enterArea();
	void leaveArea();

	void handleEvents();

	void handleActions();
	void handlePCMovement();
};

} // End of namespace KotOR2

} // End of namespace Engines

#endif // ENGINES_KOTOR2_MODULE_H
