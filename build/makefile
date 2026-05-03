main: main.o combatRoom.o dodge.o random_event.o entities.o healing.o output_text.o player.o Map.o room.o weapon.o railgun.o save.o
	g++ $^ -o $@
main.o: ../main.cpp ../room/Map.h ../room/room.h ../player/player.h ../healings/healing.h ../weapons/weapon.h ../output_text/output_text.h ../save/save.h
	g++ -c $< -o $@
combatRoom.o: ../combat/combatRoom.cpp ../combat/combatRoom.h ../player/player.h ../healings/healing.h ../weapons/weapon.h ../entities/entities.h ../combat/dodge.h ../combat/random_event.h ../output_text/output_text.h
	g++ -c $< -o $@
dodge.o: ../combat/dodge.cpp ../combat/dodge.h ../output_text/output_text.h
	g++ -c $< -o $@
random_event.o: ../combat/random_event.cpp ../combat/random_event.h ../player/player.h ../healings/healing.h ../weapons/weapon.h ../entities/entities.h ../output_text/output_text.h
	g++ -c $< -o $@
entities.o: ../entities/entities.cpp ../entities/entities.h ../output_text/output_text.h
	g++ -c $< -o $@
healing.o: ../healings/healing.cpp ../healings/healing.h ../output_text/output_text.h
	g++ -c $< -o $@
output_text.o: ../output_text/output_text.cpp ../output_text/output_text.h
	g++ -c $< -o $@
player.o: ../player/player.cpp ../player/player.h ../healings/healing.h ../weapons/weapon.h ../output_text/output_text.h
	g++ -c $< -o $@
Map.o: ../room/Map.cpp ../room/Map.h ../room/room.h ../player/player.h ../weapons/weapon.h ../healings/healing.h
	g++ -c $< -o $@
room.o: ../room/room.cpp ../room/room.h ../player/player.h ../weapons/weapon.h ../healings/healing.h ../output_text/output_text.h ../combat/combatRoom.h ../entities/entities.h
	g++ -c $< -o $@
weapon.o: ../weapons/weapon.cpp ../weapons/weapon.h ../output_text/output_text.h
	g++ -c $< -o $@
railgun.o: ../weapons/railgun.cpp ../weapons/weapon.h ../output_text/output_text.h
	g++ -c $< -o $@
save.o: ../save/save.cpp ../save/save.h ../room/Map.h ../player/player.h ../room/room.h ../weapons/weapon.h ../healings/healing.h
	g++ -c $< -o $@
clean:
	rm -f *.o main
.PHONY: clean
