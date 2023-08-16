import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DevicesSelectionComponent } from './devices-selection.component';

describe('DevicesSelectionComponent', () => {
  let component: DevicesSelectionComponent;
  let fixture: ComponentFixture<DevicesSelectionComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [DevicesSelectionComponent]
    });
    fixture = TestBed.createComponent(DevicesSelectionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
