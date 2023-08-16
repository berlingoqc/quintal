import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { DevicesSelectionComponent } from './pages/devices-selection/devices-selection.component';
import { DeviceControlComponent } from './components/device-control/device-control.component';

const routes: Routes = [
  {
    path: '',
    component: DevicesSelectionComponent
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
