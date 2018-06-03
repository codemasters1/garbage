HotKeySet("{HOME}", "Trool")

Func Trool()
   Send('Albo ma, ale nie są one z krwi i kości. Jej blade i bardzo duże mięsno-kostne uda kończą się w łydkach, a stopy wykonane są z jakiegoś czarnego, metalowego materiału lub tworzywa sztucznego.');
EndFunc

While True
   Opt("SendKeyDelay", Random(50, 300))
   Sleep(10)
WEnd