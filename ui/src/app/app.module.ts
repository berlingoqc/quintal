import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { DevicesSelectionComponent } from './pages/devices-selection/devices-selection.component';
import { DeviceControlComponent } from './components/device-control/device-control.component';

@NgModule({
  declarations: [
    AppComponent,
    DevicesSelectionComponent,
    DeviceControlComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
