note over Driver, GPS2: activate
Driver->GPS2: activate(3,4,5)
GPS2 -> DataStore2: temp_a = 3
DataStore2 --> GPS2
GPS2 -> DataStore2: temp_b = 4
DataStore2 --> GPS2
GPS2 -> DataStore2: temp_c = 5
DataStore2 --> GPS2
GPS2 -> EFSM : getCurrentState()
EFSM-->GPS2: ST
GPS2 -> ST : activate
ST -> OP: GetStoreData()
OP-->ST: StoreData()
ST->StoreData:
StoreData->DataStore2: getTemp_a()
DataStore2 --> StoreData: temp_a
StoreData->DataStore2: setA(temp_a)
DataStore2-->StoreData
StoreData->DataStore2: getTemp_b()
DataStore2 --> StoreData: temp_b
StoreData->DataStore2: setB(temp_b)

StoreData->DataStore2: getTemp_c()
DataStore2 --> StoreData: temp_c
StoreData->DataStore2: setC(temp_c)

DataStore2-->StoreData
ST -> EFSM: changeState(S0)
EFSM->EFSM : currentState = S0
EFSM-->ST
ST-->GPS2
GPS2-->Driver


note over Driver, GPS2: Start
Driver->GPS2: Start
GPS2 -> EFSM : getCurrentState()
EFSM-->GPS2: S0
GPS2 -> S0: Start
S0-> OP: getPayMsg2()
OP -->S0: PayMsg2
S0 -> PayMsg2: payMsg()
PayMsg2-> PayMsg2:Pay Cash Message printed
PayMsg2-->S0
S0-> EFSM: changeState(S1)
EFSM->EFSM : currentState = S1
EFSM-->S0
S0-->GPS2
GPS2-->Driver





note over Driver, GPS2: PayCash(6)
Driver->GPS2: PayCash
GPS2->DataStore2: temp_Cash=6
DataStore2-->GPS2
GPS2 -> EFSM : getCurrentState()
EFSM-->GPS2: S1
GPS2->S1
S1->OP: getStoreCash
OP->S1: StoreCash
S1->StoreCash: storeCash
StoreCash->DataStore2: getTempCash()
DataStore2-->StoreCash: temp_Cash
StoreCash->DataStore2:SetCash(temp_Cash)
DataStore2-->StoreCash
StoreCash-->S1

S1->OP: getDisplayMenu
OP->S1: DisplayMenu
S1->DisplayMenu: displayMenu
DisplayMenu->DisplayMenu: displays Menu
DisplayMenu-->S1:

S1-> EFSM: changeState(S3)
EFSM->EFSM : currentState = S3
EFSM-->S1
S1-->GPS2
GPS2-->Driver






mdaEfsm.currentState.SelectGas(3);


note over Driver, GPS2: Premium
Driver->GPS2: Premium
GPS2 -> EFSM : getCurrentState()
EFSM--> GPS2 : S3
GPS2 -> S3: SelectGas(2)
S3->OP: GetSetPrice
OP-->S3: SetPrice
S3->SetPrice: setPrice(2)
SetPrice->DataStore2: getB
DataStore2 --> SetPrice : b
SetPrice->DataStore2: setPrice(b)
DataStore2 --> SetPrice :
SetPrice-->OP
OP-->S3
S3-> EFSM: changeState(S4)
EFSM->EFSM : currentState = S4
EFSM-->S3
S3-->GPS2
GPS2-->Driver




note over Driver, GPS2: Regular
Driver->GPS2: Regular
GPS2 -> EFSM : getCurrentState()
EFSM--> GPS2 : S3
GPS2 -> S3: Regular()
S3->OP: GetSetPrice
OP-->S3: SetPrice
S3->SetPrice: setPrice(1)
SetPrice->DataStore2: getA
DataStore2 --> SetPrice : a
SetPrice->DataStore2: setPrice(a)
DataStore2 --> SetPrice :
SetPrice-->OP
OP-->S3
S3-> EFSM: changeState(S4)
EFSM->EFSM : currentState = S4
EFSM-->S3
S3-->GPS2
GPS2-->Driver


note over Driver, GPS2: StartPump
Driver->GPS2: StartPump
GPS2 -> EFSM : getCurrentState()
EFSM--> GPS2 : S4
GPS2 -> S4: StartPump()

S4->OP: GetSetInitialValues
OP-->S4: SetInitialValues
S4->SetInitialValues: setInitialValues()
SetInitialValues->DataStore2: set G=0
DataStore2-->SetInitialValues:
SetInitialValues->DataStore2: set Total=0
DataStore2-->SetInitialValues:
SetInitialValues->S4:


S4->OP: Get ReadyMsg
OP-->S4: ReadyMsg
S4->ReadyMsg: readyMsg()
ReadyMsg->ReadyMsg: disply readyMsg
ReadyMsg->S4:


S4-> EFSM: changeState(S5)
EFSM->EFSM : currentState = S5
EFSM-->S4
S4-->GPS2
GPS2-->Driver


int max = (ds.getG()+1) * ds.getPrice();
    Helper.debug("Cash = "+ds.getCash()+", maxTotal="+max);
    if(ds.getCash() < max)
      mdaEfsm.currentState.StopPump();
    else
      mdaEfsm.currentState.Pump();





      note over Driver, GPS2: PumpLiter
      Driver->GPS2: PumpLiter
      GPS2->DataStore2: getCash
      DataStore2-->GPS2:
      GPS2->DataStore2: getPrice
      DataStore2-->GPS2:
      GPS2->DataStore2: getG
      DataStore2-->GPS2:
      GPS2->GPS2:  check condtion\n max = G+1 x Price
      GPS2 -> EFSM : getCurrentState()
      EFSM--> GPS2 : S5
      GPS2 -> S5: PumpLiter()

      S5->OP: getPumpLiterUnit2
      OP-->S5: PumpLiterUnit2
      S5-> PumpLiterUnit2: PumpLiterUnit2()
      PumpLiterUnit2->PumpLiterUnit2: Pump Gas
      PumpLiterUnit2-->S5

      S5->OP: gasPumpedMsg()
      OP-->S5: LitterPumped2
      S5-> LitterPumped2: gasPumpedMsg()


note over Driver, GPS2: PumpLiter
Driver->GPS2: PumpLiter
GPS2->DataStore2: getCash
DataStore2-->GPS2:
GPS2->DataStore2: getPrice
DataStore2-->GPS2:
GPS2->DataStore2: getG
DataStore2-->GPS2:
GPS2->GPS2:  check condtion\n max = G+1 x Price
GPS2 -> EFSM : getCurrentState()
EFSM--> GPS2 : S5
GPS2 -> S5: PumpLiter()

S5->OP: getPumpLiterUnit2
OP-->S5: PumpLiterUnit2
S5-> PumpLiterUnit2: PumpLiterUnit2()
PumpLiterUnit2->PumpLiterUnit2: Pump Gas
PumpLiterUnit2-->S5

S5->OP: gasPumpedMsg()
OP-->S5: LitterPumped2
S5-> LitterPumped2: gasPumpedMsg()
LitterPumped2->DataStore2: getG
DataStore2-->LitterPumped2: G
LitterPumped2->DataStore2: getPrice
DataStore2-->LitterPumped2: G
LitterPumped2->DataStore2: setTotal
DataStore2-->LitterPumped2:
LitterPumped2-->S5

S5-->GPS2
GPS2-->Driver



GasPumpedMsg->DataStore: set G=G+1
DataStore-->GasPumpedMsg:
GasPumpedMsg-->GasPumpedMsg: display pumped msg
GasPumpedMsg-->S5:

S5-->GPS2
GPS2-->Driver











note over Driver, GPS2: StopPump
Driver->GPS2: StopPump
GPS2 -> EFSM : getCurrentState()
EFSM--> GPS2 : S5
GPS2 -> S5: StopPump()

S5->OP: getStopMsg
OP-->S5: StopMsg
S5->StopMsg: StopPump
StopMsg->StopMsg: Stop Msg
StopMsg-->S5





S5-> EFSM: changeState(S6)
EFSM->EFSM : currentState = S6
EFSM-->S6
S6-->GPS2
GPS2-->Driver
-------------


note over Driver, GPS2: PumpLiter
Driver->GPS2: PumpLiter
GPS2->DataStore2: getCash
DataStore2-->GPS2:
GPS2->DataStore2: getPrice
DataStore2-->GPS2:
GPS2->DataStore2: getG
DataStore2-->GPS2:
GPS2->GPS2:  check condtion\n max = G+1 x Price
GPS2 -> EFSM : getCurrentState()
EFSM--> GPS2 : S5
GPS2 -> S5: PumpLiter()

S5->OP: getPumpLiterUnit2
OP-->S5: PumpLiterUnit2
S5-> PumpLiterUnit2: PumpLiterUnit2()
PumpLiterUnit2->PumpLiterUnit2: Pump Gas
PumpLiterUnit2-->S5

S5->OP: gasPumpedMsg()
OP-->S5: LitterPumped2
S5-> LitterPumped2: gasPumpedMsg()
LitterPumped2->DataStore2: getG
DataStore2-->LitterPumped2: G
LitterPumped2->DataStore2: getPrice
DataStore2-->LitterPumped2: Price
LitterPumped2->LitterPumped2: calculate total
LitterPumped2->DataStore2: setTotal
DataStore2-->LitterPumped2:
LitterPumped2-->S5


S5-->GPS2
GPS2-->Driver

note over Driver, GPS2: PumpLiter
Driver->GPS2: PumpLiter
Driver->GPS2: PumpLiter
GPS2->DataStore2: getCash
DataStore2-->GPS2:
GPS2->DataStore2: getPrice
DataStore2-->GPS2:
GPS2->DataStore2: getG
DataStore2-->GPS2:
GPS2->GPS2:  check condtion\n max = G+1 x Price
GPS2 -> EFSM : getCurrentState()
EFSM--> GPS2 : S5
GPS2->S5: StopPump()
S5-> OP: getStopMsg
OP-->S5: StopMsg
S5->StopMsg: stopMsg
StopMsg->StopMsg: display stop msg
StopMsg->S5
S5->EFSM: changeState(S6)
EFSM->EFSM: currentState = S6
EFSM-->S5
S5-->Driver


note over Driver, GPS2: NoReciept

Driver->GPS2: NoReciept
GPS2->EFSM: getCurrentState()
EFSM--> GPS2: S6
S6->OP:getReturnCash
OP-->S6: RetrunCash
S6->ReturnCash: returnCash
ReturnCash->DataStore2: getCash
DataStore2-->ReturnCash: cash
ReturnCash->DataStore2: getTotal
DataStore2-->ReturnCash: total
ReturnCash->ReturnCash: change = 2/n print change
ReturnCash-->S6:
S6->EFSM: changeState(S0)
EFSM->EFSM: currentState = S0
EFSM-->S6
S6-->Driver
